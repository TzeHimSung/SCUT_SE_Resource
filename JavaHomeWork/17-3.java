import java.io.*;
import java.util.*;

public class MainProgram{
    public static void main(String[] args) throws IOException {
        FileOutputStream output=new FileOutputStream("Exercise17_02.dat");
        for (int i=1;i<=100;i++){
            int x=(int)(Math.random()*100+1);
            output.write(x);
        }
        output.close();
        FileInputStream input=new FileInputStream("Exercise17_02.dat");
        int sum=0,x;
        while ((x=input.read())!=-1)
            sum+=x;
        System.out.println(sum);
        input.close();
    }
}