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

    public boolean deposit(double amt){
        balance+=amt;
        return true;
    }

    public boolean withdraw(double amt){
        if (balance>amt){
            balance-=amt;
            return true;
        }
        else
            return false;
    }
}