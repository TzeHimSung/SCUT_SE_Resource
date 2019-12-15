import java.util.*;

public class MainProgram{
    public static void main(String[] args) {
        //test code
    }
}

class MyStringBuilder1{
    public int maxSize=0;
    private int maxLength=100010;
    private char[] element=new char[maxLength];
    //实现方法
    public MyStringBuilder1(String s1){
        element=s1.toCharArray();
        maxSize=s1.length()*2;
        if (maxSize>maxLength) maxSize=maxLength;
    }

    public MyStringBuilder1 append(MyStringBuilder1 s) {
        return new MyStringBuilder1(element.toString()+s);
    }

    public MyStringBuilder1 append(int i) {
        return new MyStringBuilder1(element.toString()+String.valueOf(i));
    }

    public int length(){
        return element.length;
    }

    public char charAt(int index) {
        if (index<element.length)
            return element[index];
        else 
            return '\0';
    }

    public MyStringBuilder1 toLowerCase(){
        return new MyStringBuilder1(element.toString().toLowerCase());
    }

    public MyStringBuilder1 substring(int begin, int end) {
        return new MyStringBuilder1(element.toString().substring(begin, end));
    }

    public String toString(){
        return element.toString();
    }
}