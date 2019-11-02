package newBankProgram.banking.DAO;

import java.io.*;
import java.util.*;
import newBankProgram.banking.domain.*;
import newBankProgram.banking.*;
import newBankProgram.*;

public class CustomerDAO
{
	private final static int NAME_SIZE=20;
    private final static int RECORD_SIZE=40;

    private RandomAccessFile raf;

    //constructor
    public CustomerDAO(){
        try{
            raf=new RandomAccessFile("customers.dat","rw");
        }catch(IOException ex){
            System.out.print("Error: "+ex);
            System.exit(0);
        }
    }

    public Customer readCustomers(long position,String f_name, String l_name) throws IOException{
        raf.seek(position);
        f_name=FixedLengthStringIO.readFixedLengthString(NAME_SIZE,raf);
        f_name=f_name.trim();
        l_name=FixedLengthStringIO.readFixedLengthString(NAME_SIZE,raf);
        l_name=l_name.trim();
        Customer tmp=new Customer(f_name,l_name);
        return tmp;
    }

    public void writeCustomer(String f_name, String l_name){
        try{
            raf.seek(raf.length());
            FixedLengthStringIO.writeFixedLengthString(f_name,NAME_SIZE,raf);
            FixedLengthStringIO.writeFixedLengthString(l_name,NAME_SIZE,raf);
        }catch (IOException ex){
            ex.printStackTrace();
        }
    }

    public RandomAccessFile getRaf(){
        return raf;
    }

    public void setRaf(RandomAccessFile r){
        raf=r;
    }

    public long length() throws IOException{
        return raf.length();
    }

    public boolean deleteFile(){
        boolean flag=false;
        try{
            raf.setLength(0);
            flag=true;
        }catch (IOException ex){}
        return flag;
    }
}
