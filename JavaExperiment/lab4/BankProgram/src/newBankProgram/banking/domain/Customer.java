package newBankProgram.banking.domain;

import java.util.*;

public class Customer implements Comparable<Customer>{
    private String firstName;
    private String lastName;
    ArrayList<Account> account=new ArrayList<Account>();

    public Customer(String f, String l) {
        firstName=f; lastName=l;
    }

    public String getFirstName() {
        return firstName;
    }

    public String getLastName(){
        return lastName;
    }

    public void addAccount(Account acc){
        account.add(acc);
    }

    public Account getAccount(int index){
        return account.get(index);
    }

    public int getNumOfAccounts(){
        return account.size();
    }

    public int compareTo(Customer rhs){
        if (this.firstName.compareTo(rhs.firstName)==0 &&  //nullPointerException
            this.lastName.compareTo(rhs.lastName)==0)
            return 0;
        else return 1;
    }
}