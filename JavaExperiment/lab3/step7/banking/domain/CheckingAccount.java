package banking.domain;

public class CheckingAccount extends Account{
    private double overdraftProtection;

    public CheckingAccount(double balance){
        super(balance);
    }

    public CheckingAccount(double balance, double protect){
        super(balance);
        this.overdraftProtection=protect;
    }

    public void withdraw(double amt) throws OverdraftException{ 
        if (super.balance>=amt)
            super.balance-=amt;
        else if (super.balance+overdraftProtection>amt){
            overdraftProtection-=amt-super.balance;
            super.balance=0;
        }
        else if (overdraftProtection<=0)
            throw new OverdraftException("No overdraft protection",amt-super.balance);
        else
            throw new OverdraftException("Insufficient funds for overdraft protection",amt-super.balance);
    }

    public double getOverdraftProtection(){
        return overdraftProtection;
    }
}