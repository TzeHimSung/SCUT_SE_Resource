package newBankProgram.banking.domain;

import java.util.*;
import java.io.IOException;
import java.io.RandomAccessFile;
import newBankProgram.banking.DAO.*;

public class Bank
{
	final static int maxSize=10;
    private Customer[] customers=new Customer[maxSize];
    private int numberOfCustomers;
    private static Bank bank=new Bank();
    private CustomerDAO customerFile=new CustomerDAO();

    private Bank(){
        numberOfCustomers=0;
    }

    public void addCustomer(String f, String l) {
        customers[numberOfCustomers++]=new Customer(f,l);
    }

    public int getNumOfCustomers() {
        return numberOfCustomers;
    }

    public Customer getCustomer(int index) {
        return customers[index];
    }

    public static Bank getBank() {
        return bank;
    }

    public void sortCustomers(){
        Arrays.sort(customers,0,numberOfCustomers,new MyComparator());
    }

    public String[] getName(){
        String[] tmp=new String[10];
        for (int i=0;i<numberOfCustomers;i++)
            tmp[i]=customers[i].getFirstName()+","+customers[i].getLastName();
        return tmp;
    }

    public int searchCustomer(String f, String l){ //problem here
        int t=-1;
        Customer tmp=new Customer(f, l);
        for (int i=0;i<customers.length;i++){
            if (tmp.compareTo(customers[i])==0){ //nullPointerExecption
                t=i;
                return t;
            }
        }
        return t;
    }

    public void deleteCustomer(int index){
        for (int i=index+1;i<numberOfCustomers;i++)
            customers[i-1]=customers[i];
        customers[numberOfCustomers-1]=new Customer("", "");
        numberOfCustomers--;
        customerFile.deleteFile();
        try{
            RandomAccessFile raf=new RandomAccessFile("customers.dat","rw");
            customerFile.setRaf(raf);
        }catch(IOException ex){
            System.out.print("Error: "+ex);
            System.exit(0);
        }
        for (int i=0;i<numberOfCustomers;i++){
            String f=customers[i].getFirstName();
            String l=customers[i].getLastName();
            customerFile.writeCustomer(f,l);
        }
    }
}

class MyComparator implements Comparator<Customer>{
    public int compare(Customer a, Customer b){
        if (a.getLastName().compareTo(b.getLastName())<0)
            return -1;
        else if (a.getLastName().compareTo(b.getLastName())==0 && a.getFirstName().compareTo(b.getFirstName())<0)
            return -1;
        else if (a.getLastName().compareTo(b.getLastName())>0)
            return 1;
        else if (a.getLastName().compareTo(b.getLastName())==0 && a.getFirstName().compareTo(b.getFirstName())>0)
            return 1;
        else 
            return 0;
    }
}
