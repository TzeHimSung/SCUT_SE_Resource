package banking;

public class Account{
    
    private double balance;

    public Account() {
        balance=0;
    }

    public Account(double init_balance){
        balance=init_balance;
    }

    public double getBalance() {
        return balance;
    }

    public double deposit(double amt){
        balance+=amt;
        return balance;
    }

    public double withdraw(double amt){
        balance-=amt;
        return balance;
    }
}