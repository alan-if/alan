import com.google.inject.Injector;
import org.eclipse.emf.common.util.URI;
import org.eclipse.emf.ecore.resource.Resource;
import org.eclipse.xtext.resource.XtextResourceSet;
import se.alanif.alan.AlanStandaloneSetup;

import java.nio.file.*;
import java.util.*;
import java.util.concurrent.*;
import java.util.stream.*;

/** Parse many .alan files, each with a hard timeout so a catastrophic-backtracking
 *  file can't hang the whole run. Reports pass / fail / hang and samples of each. */
public class AlanBatch {
    public static void main(String[] args) throws Exception {
        Injector injector = new AlanStandaloneSetup().createInjectorAndDoEMFRegistration();
        long timeoutMs = args.length > 1 ? Long.parseLong(args[1]) : 5000;
        List<Path> files;
        try (Stream<Path> s = Files.walk(Paths.get(args[0]))) {
            files = s.filter(p -> p.toString().endsWith(".alan")).sorted().collect(Collectors.toList());
        }
        int ok = 0, fail = 0, hang = 0;
        List<String> failSamples = new ArrayList<>(), hangFiles = new ArrayList<>();
        ExecutorService exec = Executors.newSingleThreadExecutor(r -> { Thread t = new Thread(r); t.setDaemon(true); return t; });

        for (Path f : files) {
            Future<Integer> fut = exec.submit(() -> {
                XtextResourceSet rs = injector.getInstance(XtextResourceSet.class);
                Resource res = rs.getResource(URI.createFileURI(f.toAbsolutePath().toString()), true);
                return res.getErrors().size();
            });
            try {
                int errs = fut.get(timeoutMs, TimeUnit.MILLISECONDS);
                if (errs == 0) ok++;
                else { fail++; if (failSamples.size() < 10) failSamples.add(f.getFileName().toString()); }
            } catch (TimeoutException te) {
                hang++; hangFiles.add(f.getFileName().toString());
                // The worker thread is stuck in CPU-bound backtracking and won't
                // interrupt; abandon it (daemon) and start a fresh executor.
                fut.cancel(true);
                exec.shutdownNow();
                exec = Executors.newSingleThreadExecutor(r -> { Thread t = new Thread(r); t.setDaemon(true); return t; });
                if (hang >= 8) { System.out.println("...stopping after 8 hangs"); break; }
            } catch (Exception e) {
                fail++; if (failSamples.size() < 10) failSamples.add(f.getFileName() + " (" + e.getClass().getSimpleName() + ")");
            }
        }
        int total = ok + fail + hang;
        System.out.printf("%n===== %d files: %d parsed (%.0f%%), %d parse-errors, %d HANGS =====%n",
                total, ok, 100.0 * ok / total, fail, hang);
        System.out.println("hang files: " + hangFiles);
        System.out.println("parse-error sample: " + failSamples);
        Runtime.getRuntime().halt(0); // don't wait on abandoned daemon threads
    }
}
