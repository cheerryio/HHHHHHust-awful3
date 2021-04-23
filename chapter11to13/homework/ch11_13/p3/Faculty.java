package homework.ch11_13.p3;

import java.util.Objects;

public class Faculty extends Person implements Cloneable {
    /**
     * 教工Id
     */
    private int facultyId;
    /**
     * 职称
     */
    private String title;
    /**
     * 邮箱
     */
    private String email;

    /**
     * 缺省构造函数
     */
    public Faculty(){

    }

    /**
     * 构造函数
     *
     * @param name
     * @param age
     * @param facultyId
     * @param title
     * @param email
     */
    public Faculty(String name,int age,int facultyId,String title,String email){
        super(name,age);
        this.facultyId=facultyId;
        this.title=title;
        this.email=email;
    }

    public int getFacultyId(){
        return this.facultyId;
    }

    public void setFacultyId(int facultyId){
        this.facultyId=facultyId;
    }

    public String getTitle(){
        return this.title;
    }

    public void setTitle(String title){
        this.title=title;
    }

    public String getEmail(){
        return this.email;
    }

    public void setEmail(String email){
        this.email=email;
    }

    @Override
    public String toString() {
        return super.toString()+"教工Id："+this.facultyId+" 职称："+this.title+" 邮箱："+this.email+"\n";
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        if (!super.equals(o)) return false;
        Faculty faculty = (Faculty) o;
        return facultyId == faculty.facultyId && Objects.equals(title, faculty.title) && Objects.equals(email, faculty.email);
    }

    @Override
    public int hashCode() {
        return Objects.hash(facultyId, title, email);
    }

    @Override
    public Object clone() throws CloneNotSupportedException {
        Faculty newObj=(Faculty)super.clone();
        if(this.title!=null){
            newObj.setTitle(new String(this.title));
        }
        if(this.email!=null){
            newObj.setEmail(new String(this.email));
        }
        return newObj;
    }
}
