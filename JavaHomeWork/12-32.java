import java.io.File;
import java.util.*;

public class MainProgram{
	//这份代码只做了2001年和2002年的数据处理，目的是掌握文件读写
    public static void main(String[] args) throws java.io.IOException{
        java.io.File file1=new File("2001.txt");
        Scanner cin=new Scanner(file1);
        final int maxn=6;
        NameInfo[] boy=new NameInfo[maxn];
        NameInfo[] girl=new NameInfo[maxn];
        System.out.println("Year     rk1     rk2     rk3     rk4     rk5     rk1     rk2     rk3     rk4     rk5");
        for (int i=1;i<=5;i++){
            int rank,boyRank,girlRank; String boyName,girlName;
            rank=cin.nextInt();
            boyName=cin.next();
            boyRank=cin.nextInt();
            girlName=cin.next();
            girlRank=cin.nextInt();
            boy[i]=new NameInfo(boyName,boyRank);
            girl[i]=new NameInfo(girlName, girlRank);
        }
        System.out.print("2001     ");
        for (int i=1;i<=5;i++) 
            System.out.print(boy[i].name+" ");
        for (int i=1;i<=5;i++)
            System.out.print(girl[i].name+" ");
        System.out.print("\n");
        cin.close();
        java.io.File file2=new File("2002.txt");
        Scanner input=new Scanner(file2);
        for (int i=1;i<=5;i++){
            int rank,boyRank,girlRank; String boyName,girlName;
            rank=input.nextInt();
            boyName=input.next();
            boyRank=input.nextInt();
            girlName=input.next();
            girlRank=input.nextInt();
            boy[i]=new NameInfo(boyName, boyRank);
            girl[i]=new NameInfo(girlName,girlRank);
        }
        System.out.print("2002     ");
        for (int i=1;i<=5;i++) 
            System.out.print(boy[i].name+" ");
        for (int i=1;i<=5;i++)
            System.out.print(girl[i].name+" ");
        System.out.print("\n");
        input.close();
    }
}

class NameInfo{
    String name;
    int times;

    public NameInfo(){
        name=""; times=0;
    }

    public NameInfo(String a, int b){
        name=a; times=b;
    }
}