import java.util.*;

public class MainProgram {
    public static void main(String[] args) {
        Scanner cin = new Scanner(System.in);
        System.out.print("Enter a credit card number as a long integer: ");
        long num = cin.nextLong();
        if (isValid(num))
            System.out.println("this credit card number is valid.");
        else
            System.out.println("this credit card number is not valid.");
        cin.close();
    }

    public static boolean isValid(long number) {
        int a=sumOfDoubleEvenPlace(number),b=sumOfOddPlace(number);
        if ((a+b)%10==0 && (prefixMatched(number, 4) || prefixMatched(number, 5) || prefixMatched(number, 37) || prefixMatched(number, 6)))
            return true;
        else 
            return false;
    }

    public static int sumOfDoubleEvenPlace(long number) { //get the result from step 2
        int re=0,cnt=0; long tmp=number;
        while (tmp>0) {
        	cnt++;
            long last1=tmp%10;
            int last=(int)last1;
            if (cnt%2==0) re+=getDigit(last*2);
            tmp/=10;
        }
        return re;
    }

    public static int getDigit(int number) { //return this num if it is a singal digit, else return the sum
        if (number<=9)  
            return number;
        int tmp=number,re=0;
        while (tmp>=10){
            re=0;
            while (tmp>0){
                re+=tmp%10;
                tmp/=10;
            }
            tmp=re;
        }
        return re;
    }

    public static int sumOfOddPlace(long number) { //return sum of odd place digits in num
        int size=getSize(number),sum=0;
        long tmp=number;
        for (int i=1;i<=size;i++)
        {
            if (i%2!=0) sum+=tmp%10;
            tmp/=10;
        }
        return sum;
    }

    public static boolean prefixMatched(long number, int d) { //return if the num d is a prefix for number
        int sizeOfD=getSize(d);
        int pre=getPrefix(number, sizeOfD);
        if (pre==d) 
            return true; 
        else 
            return false;
    }

    public static int getSize(long d) { //return the num of digits in d
        long tmp=d; int cnt=0;
        while (tmp>0){
            cnt++;
            tmp/=10;
        }
        return cnt;
    }

    public static int getPrefix(long number, int k) { //return the first k num of digits from num
        int size=getSize(number);
        if (size<k)
            return (int)number;
        long re=number;
        for (int i=1;i<=size-k;i++) re/=10;
        return (int)re;
    }
}
