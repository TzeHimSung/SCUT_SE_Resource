import java.util.*;

public class MainProgram{
    public static void main(String[] args) {
        Course a=new Course("math");
        Course b=(Course)a.clone();
        System.out.println(b.getCourseName());
    }
}

class Course implements Cloneable{
    private String courseName;
    private String[] students=new String[100];
    private int numberOfStudents;
    
    public Course()
	{
		courseName="";
	}

    public Course(String courseName){
        this.courseName=courseName;
    }

    public void addStudent(String student){
        students[numberOfStudents++]=student;
    }

    public int getNumberOfStudents() {
        return numberOfStudents;
    }

    public String getCourseName() {
        return courseName;
    }

    public Object clone(){
        try{
            return super.clone();
        }
        catch(CloneNotSupportedException ex){
            return null;
        }
    }
}