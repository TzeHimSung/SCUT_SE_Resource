package banking.domain;

public class Account{
    
    protected double balance;

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

    public void withdraw(double amt) throws OverdraftException{
        if (balance>=amt){
            balance-=amt;
        }
        else
            throw new OverdraftException("Insufficient funds",amt-balance);
    }
}