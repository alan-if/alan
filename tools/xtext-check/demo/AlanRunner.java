import com.google.inject.Injector;
import org.eclipse.emf.common.util.URI;
import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EStructuralFeature;
import org.eclipse.emf.ecore.resource.Resource;
import org.eclipse.xtext.diagnostics.Severity;
import org.eclipse.xtext.resource.XtextResource;
import org.eclipse.xtext.resource.XtextResourceSet;
import org.eclipse.xtext.validation.CheckMode;
import org.eclipse.xtext.validation.IResourceValidator;
import org.eclipse.xtext.validation.Issue;
import se.alanif.alan.AlanStandaloneSetup;

import java.util.List;

/** Parse a .alan file with the generated Xtext parser and print the outline model. */
public class AlanRunner {
    public static void main(String[] args) {
        Injector injector = new AlanStandaloneSetup().createInjectorAndDoEMFRegistration();
        XtextResourceSet rs = injector.getInstance(XtextResourceSet.class);
        Resource res = rs.getResource(URI.createFileURI(args[0]), true);

        System.out.println("=== parse of " + args[0] + " ===");
        if (!res.getErrors().isEmpty()) {
            System.out.println("PARSE ERRORS (" + res.getErrors().size() + "):");
            res.getErrors().forEach(e -> System.out.println("  line " + e.getLine() + ": " + e.getMessage()));
        } else {
            System.out.println("parsed with 0 syntax errors");
        }

        if (res.getContents().isEmpty()) { System.out.println("(no model)"); return; }

        System.out.println("\n=== outline model ===");
        print(res.getContents().get(0), 0);
    }

    /** Generic EMF walk: print each EObject's EClass and its 'name' feature if present. */
    static void print(EObject o, int depth) {
        EClass c = o.eClass();
        EStructuralFeature nameF = c.getEStructuralFeature("name");
        Object name = nameF != null ? o.eGet(nameF) : null;
        String label = c.getName() + (name != null ? " '" + name + "'" : "");
        System.out.println("  ".repeat(depth) + label);
        for (EObject child : o.eContents()) print(child, depth + 1);
    }
}
