import java.util.Base64;
import java.util.regex.Matcher; // https://www.w3schools.com/java/java_regex.asp
import java.util.regex.Pattern;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class PasswordManagment
{
    private static Pattern _passwordRegex;
    private static Pattern _invalidCharRegex;

    private static Pattern GetPasswordInvalidCharRegex()
    {
        if (_invalidCharRegex == null)
        {
            _invalidCharRegex = Pattern.compile("['\"?;\\s<>\\\\]");
        }
        return _invalidCharRegex;
    }

    private static Pattern GetPasswordRegex()
    {
        if (_passwordRegex == null)
        {
            // https://stackoverflow.com/questions/19605150/regex-for-password-must-contain-at-least-eight-characters-at-least-one-number-a
            // Minimum eight characters, at least one letter, one number and one special character
            _passwordRegex = Pattern.compile("^(?=.*?[A-Z])(?=.*?[a-z])(?=.*?[0-9])(?=.*?[#?!@$%^&*-]).{8,}$");
        }
        return _passwordRegex;
    }


    public record PasswordValidationResult(Boolean IsValid, String Result){ }

    public static PasswordValidationResult PasswordValidation(String password)
    {
        if (password == null || password.length() < 6)
            return new PasswordValidationResult(false, "Password have at least 8 characters");

        if (password.length() > 64)
            return new PasswordValidationResult(false, "Password must not exceed 64 characters");

        //var matcher = GetPasswordInvalidCharRegex().matcher(password);
        var matcher = GetPasswordRegex().matcher(password);
        if (!matcher.find())
            return new PasswordValidationResult(false, "Password requires 1 UPPER, 1 lower, a number [0-9] and 1 special character [#?!@$%^&*-]");

        return new PasswordValidationResult(true, password);
    }

    public static String GetSecurePassword(String password, byte[] salt)
    {
        String securePassword = null;
        try
        {
            MessageDigest md = MessageDigest.getInstance("SHA-256");
            md.update(salt);
            byte[] bytes = md.digest(password.getBytes());
            securePassword = Base64.getEncoder().encodeToString(bytes);
        }catch (NoSuchAlgorithmException e)
        {
            e.printStackTrace();
        }
        return securePassword;
    }
}
