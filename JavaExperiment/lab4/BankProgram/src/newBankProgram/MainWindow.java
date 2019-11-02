package newBankProgram;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;
import newBankProgram.banking.*;
import newBankProgram.banking.domain.*;
import newBankProgram.banking.DAO.*;
import javax.swing.*;
import javax.swing.border.*;

public class MainWindow extends JFrame {
    // define const
    final static int NAME_SIZE = 20;
    final static int RECORD_SIZE = 40;

    // access data.dat using randomaccessFile
    private RandomAccessFile raf;

    // define fields and buttons
    private JButton btnAdd = new JButton("Add");
    private JButton btnDel = new JButton("Del");
    private JButton btnSearch = new JButton("Search");
    private JButton btnSort = new JButton("Sort");
    private JTextField tfFirstName = new JTextField(NAME_SIZE);
    private JTextField tfLastName = new JTextField(NAME_SIZE);

    // get bank
    private Bank bank = Bank.getBank();

    @SuppressWarnings("unchecked")
    public MainWindow(){

        //set button panel
        JPanel panButton=new JPanel();
        panButton.setLayout(new GridLayout(1,4));
        panButton.add(btnAdd);
        panButton.add(btnDel);
        panButton.add(btnSearch);
        panButton.add(btnSort);

        //set panel of info
        JPanel panName=new JPanel();
        panName.setBorder(BorderFactory.createLoweredBevelBorder());
        JPanel p1=new JPanel();
        p1.add(new JLabel("FirstName"),BorderLayout.CENTER);
        p1.add(tfFirstName,BorderLayout.EAST);
        JPanel p2=new JPanel();
        p2.add(new JLabel("LastName"),BorderLayout.CENTER);
        p2.add(tfLastName,BorderLayout.EAST);
            
        //set panel of the right
        panName.setLayout(new GridLayout(2,1,5,15));
        panName.add(p1);
        panName.add(p2);

        //set right layout
        JPanel panRight=new JPanel();
        panRight.setLayout(new BorderLayout());
        panRight.add(panName,BorderLayout.CENTER);
        panRight.add(panButton,BorderLayout.SOUTH);

        //set left panel
        JPanel panLeft=new JPanel();
        String[] cus=bank.getName();
        JList panList=new JList(cus);
        panList.setSize(300,200);
        panList.setVisibleRowCount(10);
        panLeft.add(panList);

        //set border of the left panel
        panLeft.setBorder(new TitledBorder("Customers List"));
        panList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        panList.setFixedCellHeight(20);
        panList.setFixedCellWidth(300);

        //set panel and add to frame
        panLeft.setSize(300,200);
        panRight.setSize(300,200);
        add(panLeft,BorderLayout.CENTER);
        add(panRight,BorderLayout.EAST);

        //sign add button listener
        btnAdd.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e){
                if ((!tfFirstName.getText().isEmpty()) || (!tfLastName.getText().isEmpty())){
                    bank.addCustomer(tfFirstName.getText(),tfLastName.getText());
                    String[] cus=bank.getName();
                    panList.setListData(cus);
                }
            }
        });

        //sign delete button listener
        btnDel.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e){
                int tmp=panList.getSelectedIndex();
                if (tmp!=-1){
                    bank.deleteCustomer(tmp);
                    String[] cus=bank.getName();
                    panList.setListData(cus);
                }  
            }
        });

        //sign search button listener
        btnSearch.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e){
                String f=tfFirstName.getText(), l=tfLastName.getText();
                int tmp=bank.searchCustomer(f,l); //exception here
                if (tmp!=-1) panList.setSelectedIndex(tmp); //if exist, show it
            }
        });

        //sign sort button listener
        btnSort.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e){
                bank.sortCustomers();
                String[] cus=bank.getName();
                panList.setListData(cus);
            }
        });
    }

    // main function
    @SuppressWarnings("unchecked")
    public static void main(String[] args) {
        MainWindow frame = new MainWindow();
        frame.setTitle("Bank Program");
        frame.pack();
        frame.setLocationRelativeTo(null);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);
    }
}