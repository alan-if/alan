import com.google.inject.Injector;
import org.eclipse.emf.common.util.URI;
import org.eclipse.emf.ecore.resource.Resource;
import org.eclipse.xtext.resource.XtextResourceSet;
import se.alanif.alan.AlanStandaloneSetup;

import java.nio.file.*;
import java.util.*;
import java.util.stream.*;

/** Parse many .alan files in one JVM; report pass/fail and a sample of failures. */
public class AlanBatch {
    public static void main(String[] args) throws Exception {
        Injector injector = new AlanStandaloneSetup().createInjectorAndDoEMFRegistration();
        List<Path> files;
        try (Stream<Path> s = Files.walk(Paths.get(args[0]))) {
            files = s.filter(p -> p.toString().endsWith(".alan")).sorted().collect(Collectors.toList());
        }
        int ok = 0, fail = 0;
        List<String> samples = new ArrayList<>();
        for (Path f : files) {
            try {
                XtextResourceSet rs = injector.getInstance(XtextResourceSet.class);
                Resource res = rs.getResource(URI.createFileURI(f.toAbsolutePath().toString()), true);
                if (res.getErrors().isEmpty()) {
                    ok++;
                } else {
                    fail++;
                    if (samples.size() < 12) {
                        var e = res.getErrors().get(0);
                        samples.add(f.getFileName() + " -> line " + e.getLine() + ": " + e.getMessage());
                    }
                }
            } catch (Throwable t) {
                fail++;
                if (samples.size() < 12) samples.add(f.getFileName() + " -> EXCEPTION " + t.getClass().getSimpleName());
            }
        }
        int total = ok + fail;
        System.out.printf("%n===== %d files: %d parsed (%.1f%%), %d failed =====%n",
                total, ok, 100.0 * ok / total, fail);
        System.out.println("sample failures:");
        samples.forEach(s -> System.out.println("  " + s));
    }
}
