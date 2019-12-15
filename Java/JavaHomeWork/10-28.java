import java.util.*;

public class MainProgram{
    public static void main(String[] args) {
        //test code
    }
}

class MyStringBuilder2{
    private int maxLength=100010;
    public int maxSize=0;
    private char[] element=new char[maxLength];

    public MyStringBuilder2(){
        element="".toCharArray();
        maxSize=element.length*2;
    }

    public MyStringBuilder2(char[] chars){
        element=chars;
        maxSize=element.length*2;
    }

    public MyStringBuilder2(String s){
        element=s.toCharArray();
        maxSize=element.length*2;
    }

    public MyStringBuilder2 insert(int offset, MyStringBuilder2 s){
        return new MyStringBuilder2(new StringBuffer(element.toString()).insert(offset,s.element).toString());
    }

    public MyStringBuilder2 reverse(){
        return new MyStringBuilder2(new StringBuffer(element.toString()).reverse().toString());
    }

    public MyStringBuilder2 substring(int begin){
        return new MyStringBuilder2(element.toString().substring(begin, element.length-1));
    }

    public MyStringBuilder2 toUpperCase(){
        return new MyStringBuilder2(element.toString().toUpperCase());
    }
}