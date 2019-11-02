import java.util.*;

public class MainProgram {
    public static void main(String[] args) {
        Scanner cin = new Scanner(System.in);
        int a = cin.nextInt(), sum = 0;
        while (a > 0) {
            sum += a % 10;
            a /= 10;
        }
        System.out.println(sum);
        cin.close();
    }
}
