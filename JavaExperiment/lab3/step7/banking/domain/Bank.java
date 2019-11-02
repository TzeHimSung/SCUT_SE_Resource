package banking.domain;
import java.util.*;

public class Bank{

    final static int maxSize=10;
    private Customer[] customers=new Customer[maxSize];
    private int numberOfCustomers;
    private static Bank bank=new Bank();

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