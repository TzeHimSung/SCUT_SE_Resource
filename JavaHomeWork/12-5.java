import java.util.*;

public class MainProgram{
    public static void main(String[] args){
        //test code
    }
}

class GeometricObject{
    private String color="white";
    private boolean filled;
    private Date dateCreated;

    public GeometricObject(){
        dateCreated=new Date();
    }

    public GeometricObject(String color,boolean filled){
        dateCreated=new Date();
        this.color=color;
        this.filled=filled;
    }

    public String getColor(){
        return color;
    }
    
    public void setColor(String color){
        this.color=color;
    }

    public boolean isFilled(){
        return filled;
    }

    public void setFilled(boolean filled){
        this.filled=filled;
    }

    public Date getDateCreated(){
        return this.dateCreated;
    }

    public String toString(){
        return "created on "+dateCreated+"\ncolor: "+color+" and filled: "+filled;
    }
}

class Triangle extends GeometricObject{
    double side1=1.0,side2=1.0,side3=1.0;

    public Triangle(){
        this.side1=1.0; this.side2=1.0; this.side3=1.0;
    }

    public Triangle(double a,double b,double c) throws IllegalTriangleException{
        if (a+b>c && b+c>a && a+c>b){
            side1=a; side2=b; side3=c;
        }
        else
            throw new IllegalTriangleException(side1,side2,side3);
    }

    public double getSide1() {return side1;}
    public double getSide2() {return side2;}
    public double getSide3() {return side3;}

    public double getArea(){
        double r=(side1+side2+side3)/2.0;
        return Math.sqrt(r*(r-side1)*(r-side2)*(r-side3));
    }

    public double getPerimeter(){
        return side1+side2+side3;
    }

    public String toString(){
        return "Triangle: side1 = "+side1+" side2 = "+side2+" side3 = "+side3;
    }
}

class IllegalTriangleException extends Exception{
	private static final long serialVersionUID = 1L;
	double s1,s2,s3;
	public IllegalTriangleException(double a, double b, double c) {
		super("Can not build triangle");
		s1=a; s2=b; s3=c;
	}
}