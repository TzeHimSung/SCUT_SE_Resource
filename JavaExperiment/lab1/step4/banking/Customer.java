package banking;

public class Customer{
    private String firstName;
    private String lastName;
    Account account=new Account();

    public Customer(String f, String l) {
        firstName=f; lastName=l;
    }

    public String getFirstName() {
        return firstName;
    }

    public String getLastName(){
        return lastName;
    }

    public Account getAccount(){
        return account;
    }

    public void setAccount(Account acct){
        this.account=acct;
    }

    
}