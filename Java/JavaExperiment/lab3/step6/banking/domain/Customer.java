package banking.domain;
import java.util.*;

public class Customer{
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
}