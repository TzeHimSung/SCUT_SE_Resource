import java.util.*;

public class MainProgram {
    public static void main(String[] args) {
        //test code
        Triangle2D tri1 = new Triangle2D(new MyPoint(2.5, 2), new MyPoint(4.2, 3), new MyPoint(5, 3.5));
        System.out.printf("The area of the first triangle is: %f\n", tri1.getArea());
        System.out.printf("The perimeter of the first triangle is: %f\n", tri1.getPerimeter());
        System.out.println(tri1.contains(new MyPoint(3, 3)));
        System.out.println(tri1.contains(new Triangle2D(new MyPoint(2.9, 2), new MyPoint(4, 1), new MyPoint(1, 3.4))));
        System.out.println(tri1.overlaps(new Triangle2D(new MyPoint(2, 5.5), new MyPoint(4, -3), new MyPoint(2, 6.5))));

    }
}

class MyPoint {
    double x, y;

    public MyPoint() {
        x = 0;
        y = 0;
    }

    public MyPoint(double a, double b) {
        x = a;
        y = b;
    }

    public double distance(MyPoint p) {
        double re = Math.sqrt(Math.pow(p.x - x, 2) + Math.pow(p.y - y, 2));
        return re;
    }

    public double distance(int px, int py) {
        double re = Math.sqrt(Math.pow(px - x, 2) + Math.pow(py - y, 2));
        return re;
    }

    public static double distance(MyPoint p1, MyPoint p2) {
        double re = Math.sqrt(Math.pow(p1.x - p2.x, 2) + Math.pow(p1.y - p2.y, 2));
        return re;
    }
}

class Triangle2D {
    final double eps = 1e-6;
    MyPoint p1, p2, p3;

    public MyPoint getP1() {
        return p1;
    }

    public MyPoint getP2() {
        return p2;
    }

    public MyPoint getP3() {
        return p3;
    }

    public void setP1(MyPoint p) {
        p1 = p;
    }

    public void setP2(MyPoint p) {
        p2 = p;
    }

    public void setP3(MyPoint p) {
        p3 = p;
    }

    public Triangle2D() {
        p1 = new MyPoint(0, 0);
        p2 = new MyPoint(1, 1);
        p3 = new MyPoint(2, 5);
    }

    public Triangle2D(MyPoint np1, MyPoint np2, MyPoint np3) {
        p1 = np1;
        p2 = np2;
        p3 = np3;
    }

    public double getArea() {
        double e1 = Math.sqrt(Math.pow(p1.x - p2.x, 2) + Math.pow(p1.y - p2.y, 2));
        double e2 = Math.sqrt(Math.pow(p2.x - p3.x, 2) + Math.pow(p2.y - p3.y, 2));
        double e3 = Math.sqrt(Math.pow(p1.x - p3.x, 2) + Math.pow(p1.y - p3.y, 2));
        double r = (e1 + e2 + e3) / 2.0;
        double re = Math.sqrt(r * (r - e1) * (r - e2) * (r - e3));
        return re;
    }

    public double getPerimeter() {
        double e1 = Math.sqrt(Math.pow(p1.x - p2.x, 2) + Math.pow(p1.y - p2.y, 2));
        double e2 = Math.sqrt(Math.pow(p2.x - p3.x, 2) + Math.pow(p2.y - p3.y, 2));
        double e3 = Math.sqrt(Math.pow(p1.x - p3.x, 2) + Math.pow(p1.y - p3.y, 2));
        double re = e1 + e2 + e3;
        return re;
    }

    public boolean contains(MyPoint p) {
        double s1 = getArea();
        double e1 = Math.sqrt(Math.pow(p1.x - p2.x, 2) + Math.pow(p1.y - p2.y, 2));
        double e2 = Math.sqrt(Math.pow(p2.x - p3.x, 2) + Math.pow(p2.y - p3.y, 2));
        double e3 = Math.sqrt(Math.pow(p1.x - p3.x, 2) + Math.pow(p1.y - p3.y, 2));
        double e4 = Math.sqrt(Math.pow(p1.x - p.x, 2) + Math.pow(p1.y - p.y, 2));
        double e5 = Math.sqrt(Math.pow(p.x - p3.x, 2) + Math.pow(p.y - p3.y, 2));
        double e6 = Math.sqrt(Math.pow(p.x - p2.x, 2) + Math.pow(p.y - p2.y, 2));
        double r1 = (e1 + e6 + e4) / 2.0, r2 = (e2 + e5 + e6) / 2.0, r3 = (e3 + e4 + e5) / 2.0;
        double s3 = Math.sqrt(r1 * (r1 - e1) * (r1 - e4) * (r1 - e6));
        double s4 = Math.sqrt(r2 * (r2 - e2) * (r2 - e5) * (r2 - e6));
        double s5 = Math.sqrt(r3 * (r3 - e3) * (r3 - e4) * (r3 - e5));
        double s2 = s3 + s4 + s5;
        if (s2 - s1 > eps)
            return false;
        else
            return true;
    }

    public boolean contains(Triangle2D tri) {
        if ((contains(tri.p1) && contains(tri.p2) && contains(tri.p3))
                || (tri.contains(p1) && tri.contains(p2) && tri.contains(p3)))
            return true;
        else
            return false;
    }

    public boolean overlaps(Triangle2D tri) {
        if ((crossProduct(p1, p2, tri.p1, tri.p2) || crossProduct(p1, p3, tri.p1, tri.p2)
                || crossProduct(p2, p3, tri.p1, tri.p2) || crossProduct(p1, p2, tri.p1, tri.p3)
                || crossProduct(p1, p3, tri.p1, tri.p3) || crossProduct(p2, p3, tri.p1, tri.p3)
                || crossProduct(p1, p2, tri.p2, tri.p3) || crossProduct(p1, p3, tri.p2, tri.p3)
                || crossProduct(p2, p3, tri.p2, tri.p3)) && !contains(tri))
            return true;
        else
            return false;
    }

    private boolean crossProduct(MyPoint p1, MyPoint p2, MyPoint p3, MyPoint p4) {
        double u, v, w, z;
        u = (p3.x - p1.x) * (p2.y - p1.y) - (p2.x - p1.x) * (p3.y - p1.y);
        v = (p4.x - p1.x) * (p2.y - p1.y) - (p2.x - p1.x) * (p4.y - p1.y);
        w = (p1.x - p3.x) * (p4.y - p3.y) - (p4.x - p3.x) * (p1.y - p3.y);
        z = (p2.x - p3.x) * (p4.y - p3.y) - (p4.x - p3.x) * (p2.y - p3.y);
        return (u * v <= 1e-8 && w * z <= 1e-8);
    }
}