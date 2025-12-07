import java.text.Format;
import java.util.Scanner;
import org.json.*;

//TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
public class Main
{

    // Escapes the following characters
    // & < > " '
    public static String EscapeXSS(String str)
    {
        return str
                .replace("&", "&amp;")
                .replace("<", "&lt;")
                .replace(">", "&gt;")
                .replace("\"", "&quot;")
                .replace("'", "&#39;")
                ;
    }

    private static void PrintAndEscapeXSS(String str)
    {
        System.out.println("Input  : " +  str);
        System.out.println("Escaped: " + EscapeXSS(str));
        System.out.println();
    }

    static void main()
    {
        // HTML Escapement
        System.out.println("\n\n------------   Showcasing XSS Escapement ------------ ");
        PrintAndEscapeXSS("<A HREF=\"http://0x42.0x0000066.0x7.0x93/\">XSS</A>");
        PrintAndEscapeXSS("<SCRIPT a=\">'>\" SRC=\"httx://xss.rocks/xss.js\"></SCRIPT>");
        PrintAndEscapeXSS("<IFRAME SRC=\"javascript:alert('XSS');\"></IFRAME>");

        System.out.println("\n------------   Password Validation ------------ ");
        Scanner scanner = new Scanner(System.in);
        while (true)
        {
            System.out.print("Enter Password or 'exit' to quit : ");
            String input = scanner.nextLine();
            if (input.equals("exit"))
            {
                break;
            }
            var result = PasswordManagment.PasswordValidation(input);
            if (result.IsValid())
                System.out.println("Password is OK");
            else
                System.out.println("Password is invalid : " + result.Result());
        }

    }
}
