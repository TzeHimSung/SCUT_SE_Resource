import java.io.*;
import java.util.*;

public class MainProgram{
    public static void main(String[] args) throws IOException{
        if (args.length<2){
            System.out.println("Syntax error!");
            System.exit(0);
        }
        int lenOfCom=args.length;
        String aimFileName=args[lenOfCom-1];
        File aimFile=new File(aimFileName);
        FileOutputStream fout=new FileOutputStream(aimFile);
        FileInputStream fin;
        for (int i=0;i<lenOfCom-1;i++){
            File sourceFile=new File(args[i]);
            fin=new FileInputStream(sourceFile);
            int value;
            while ((value=fin.read())!=-1)
                fout.write(value);
            fin.close();
        }
        fout.close();
    }
}