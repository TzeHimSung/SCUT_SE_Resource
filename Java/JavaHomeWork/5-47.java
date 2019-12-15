import java.util.*;

public class MainProgram {
    public static int calc(String s) {
        int sum = 0;
        char ss[] = s.toCharArray();
        for (int i = 0; i < s.length(); i++)
            if (i % 2 != 0)
                sum += s.charAt(i) - '0';
            else
                sum += 3 * (s.charAt(i) - '0');
        sum %= 10;
        int re = 10 - sum;
        return re;
    }

    public static void main(String[] args) {
        Scanner cin = new Scanner(System.in);
        System.out.print("Enter the first 12 digits of an ISBN-13 as a string: ");
        String s = cin.nextLine();
        System.out.print("The ISBN-13 number is ");
        int d13 = calc(s);
        System.out.print(s);
        System.out.print(d13);
        cin.close();
    }
}
