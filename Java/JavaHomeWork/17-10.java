import java.io.*;
import java.util.*;

public class MainProgram{
    public static void main(String[] args) throws IOException{
        if (args.length<2){
            System.out.println("Command format error! Please input [file name] [number of files]");
            System.exit(0);
        }
        File sourceFile=new File(args[0]);
        if (!sourceFile.exists()){
            System.out.println("The file is not exist!");
            System.exit(0);
        }
        int numOfFile=Integer.parseInt(args[1]);
        FileReader fileReader=new FileReader(sourceFile);
        BufferedReader bufR=new BufferedReader(fileReader);
        String row; int numOfRow=0;
        while ((row=bufR.readLine())!=null)
            numOfRow++;
        bufR.close();
        BufferedReader bufR2=new BufferedReader(fileReader);
        int signNum=numOfRow/numOfFile,fileNo=1;
        FileWriter fileWriter=new FileWriter(sourceFile.getParent()+fileNo+".txt");
        while ((row=bufR2.readLine())!=null){
            numOfRow++;
            fileWriter.append(row+"\r\n");
            if ((numOfRow/signNum)>(fileNo-1)){
                fileWriter.close();
                fileNo++;
                fileWriter=new FileWriter(sourceFile.getParent()+fileNo+".txt");
            }
        }
        fileWriter.close();
        System.out.println("The file is divided successfully!");
    }
}