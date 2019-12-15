import java.util.*;

public class MainProgram
{
	public static void main(String[] args)
	{
		Scanner cin=new Scanner(System.in);
		System.out.print("The amount invested: ");
		double inv=cin.nextDouble();
		System.out.print("Annual interest rate: ");
		double rate=cin.nextDouble();
		futureInvestmentValue(inv, rate, 30);
		cin.close();
	}
	
	public static void futureInvestmentValue(double investmentAmount, double monthlyInterestRate,int years){
		double rate=monthlyInterestRate/100.0/12.0+1.0;
		System.out.printf("Years     Future Value\n");
		for (int i=1;i<=30;i++) {
			double ans=investmentAmount*Math.pow(rate, i*12);
			System.out.printf("%d        %f\n",i,ans);
		}
	}
}
