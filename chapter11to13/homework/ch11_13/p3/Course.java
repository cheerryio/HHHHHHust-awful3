package homework.ch11_13.p3;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

public class Course implements Cloneable {
    /**
     * 课程名称
     */
    private String courseName;
    /**
     * 选修课程的学生列表，保存在 ArrayList里
     */
    private List<Person> students=new ArrayList<Person>();
    /**
     * 课程的授课老师
     */
    private Person teacher;

    /**
     * 构造函数
     *
     * @param courseName
     * @param teacher
     */
    public Course(String courseName,Person teacher){
        this.courseName=courseName;
        this.teacher=teacher;
    }

    /**
     * 选修课程。 应该把选修的学生加入到学生列表里。注意同一个学生只能选修一次，内
     * 部的 ArrayList 里不能出现重复的学生
     *
     * @param s  选修课程的学生
     */
    public void register(Person s){
        if(!this.students.contains(s)){
            this.students.add(s);
        }
    }

    /**
     * 取消选修 应该把取消选修的学生从学生名单里删除
     *
     * @param s 取消选修的学生
     */
    public void unregister(Person s){
        this.students.remove(s);
    }

    public String getCourseName(){
        return this.courseName;
    }

    public List<Person> getStudents(){
        return this.students;
    }

    public Person getTeacher(){
        return this.teacher;
    }

    /**
     * 获取选修课程的学生总数
     *
     * @return
     */
    public int getNumberOfStudent(){
        return this.students.size();
    }

    @Override
    public String toString() {
        StringBuffer s=new StringBuffer("");
        s.append("课程名称："+this.courseName);
        s.append("学生:");
        for(Person student:this.students){
            s.append(student.toString());
            s.append("--");
        }
        s.append("老师："+this.teacher);
        return s.toString();
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Course course = (Course) o;
        return Objects.equals(courseName, course.courseName) &&
                Objects.equals(teacher, course.teacher) &&
                this.students.size()==course.getNumberOfStudent() &&
                this.students.containsAll(course.students);
    }

    @Override
    public int hashCode() {
        return Objects.hash(courseName, students, teacher);
    }

    @Override
    public Object clone() throws CloneNotSupportedException {
        Course newObj=(Course)super.clone();
        newObj.courseName=new String(this.courseName);
        List<Person> newStudents=new ArrayList<Person>(this.students.size());
        for(Person s:this.students){
            newStudents.add((Person)s.clone());
        }
        newObj.students=newStudents;
        newObj.teacher=(Person)this.teacher.clone();
        return newObj;
    }
}
