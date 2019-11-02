import java.util.*;

public class MainProgram{
    public static void main(String[] args) {
        //use to test
    }
}

class Account{
    
    int id=0;
    double balance=0;
    double annualInterestRate=0;
    Date dateCreated;

    //创建默认账户的无参构造方法
    public Account(){
        id=0;
        balance=0;
        annualInterestRate=0;
    }

    //创建具有指定id和初始余额的账户的构造方法
    public Account(int newID, double bal){
        id=newID;
        balance=bal;
        annualInterestRate=0;
    }

    //id balance和annualIntererstRate访问器方法 修改器方法
    public int id() {
        return id;
    }

    public double getBalance() {
        return balance;
    }

    public double getAnnualInterestRate() {
        return annualInterestRate;
    }

    public void setId(int newId) {
        id=newId;
    }

    public void setBalance(double newBal) {
        balance=newBal;
    }

    public void setAnnualInterestRate(double newRate) {
        annualInterestRate=newRate;
    }

    //dateCreated访问器方法
    public Date getDateCreated() {
        return dateCreated;
    }

    //double getMonthlyInterestRate() 返回月利率
    public double getMonthlyInterestRate() {
        return annualInterestRate/100.0/12.0;
    }

    //double getMonthlyInterest() 返回月利息
    public double getMonthlyInterest() {
        return balance*getMonthlyInterestRate();
    }

    //withDraw 提取指定额度
    public double withDraw(double mon) {
        balance-=mon;
        return mon;
    }

    //deposit 存储指定额度
    public double deposit(double mon) {
        balance+=mon;
        return mon;
    }
}