import java.util.*;

public class MainProgram
{
	public static void main(String[] args)
	{
		Scanner cin=new Scanner(System.in);
		System.out.println("Please input the X coordinate: ");
		double x=cin.nextDouble();
		System.out.println("Please input the Y coordinate: ");
		double y=cin.nextDouble();
		if (x<0 || y<0)
			System.out.println("The point is not in the triangle");
		else {
			double k=0.5*x+y-100;
			if (k>=0)
				System.out.println("The point is not in the triangle");
			else
				System.out.println("The point is in the triangle");
		}
		cin.close();
	}
}
