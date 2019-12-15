import java.io.*;
import java.util.*;

public class MainProgram{
    public static void main(String[] args) throws IOException{
        System.out.print("Please input the source file name: ");
        Scanner cin=new Scanner(System.in);
        String sourceFileName=cin.next();
        System.out.print("Please input the aim file name: ");
        String aimFileName=cin.next();
        File sourceFile=new File(sourceFileName);
        if (!sourceFile.exists()){
            System.out.println("The source file is not exist!");
            System.exit(0);
        }
        File aimFile=new File(aimFileName);
        FileInputStream fin=new FileInputStream(sourceFile);
        FileOutputStream fout=new FileOutputStream(aimFile);
        int value;
        while ((value=fin.read())!=-1)
            fout.write(value-5);
        fin.close();
        fout.close();
    }
}