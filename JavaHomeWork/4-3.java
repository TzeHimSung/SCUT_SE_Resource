import java.util.*;

public class MainProgram {
    static final double r = 6371.01;

    public static double calDis(double x1, double y1, double x2, double y2) {
        double ans = 0;
        ans = r * Math.acos(Math.sin(Math.toRadians(x1)) * Math.sin(Math.toRadians(x2)) + Math.cos(Math.toRadians(x1))
                * Math.cos(Math.toRadians(x2)) * Math.cos(Math.toRadians(y1) - Math.toRadians(y2)));
        return ans;
    }

    public static void main(String[] args) {
        double x1 = 33.7489954, y1 = -84.3879824;
        double x2 = 28.5383355, y2 = -81.3792365;
        double x3 = 32.0835407, y3 = -81.0998342;
        double x4 = 35.2270869, y4 = -80.8431267;
        double e1 = calDis(x1, y1, x2, y2), e2 = calDis(x2, y2, x3, y3);
        double e3 = calDis(x1, y1, x4, y4), e4 = calDis(x3, y3, x4, y4), e5 = calDis(x1, y1, x3, y3);
        double r1 = (e1 + e2 + e5) / 2.0, r2 = (e3 + e4 + e5) / 2.0;
        double s1 = Math.sqrt(r1 * (r1 - e1) * (r1 - e2) * (r1 - e5)),
                s2 = Math.sqrt(r2 * (r2 - e3) * (r2 - e4) * (r2 - e5));
        double ans = s1 + s2;
        System.out.printf("The area is: %f\n", ans);
    }
}
