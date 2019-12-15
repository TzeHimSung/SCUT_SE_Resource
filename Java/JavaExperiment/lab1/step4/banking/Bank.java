package banking;

public class Bank{

    final static int maxSize=10;
    private Customer[] customers=new Customer[maxSize];
    private int numberOfCustomers;

    public Bank(){
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
}