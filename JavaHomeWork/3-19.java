import java.util.*;

public class MainProgram {
    public static void main(String[] args) {
        Scanner cin = new Scanner(System.in);
        final double eps = 1e-8;
        double a = cin.nextDouble(), b = cin.nextDouble(), c = cin.nextDouble();
        if (a + b - c > eps && b + c - a > eps && a + c - b > eps)
            System.out.printf("The perimiter of the triangle is: %f\n", a + b + c);
        else
            System.out.println("Not a triangle!");
        cin.close();
    }
}
