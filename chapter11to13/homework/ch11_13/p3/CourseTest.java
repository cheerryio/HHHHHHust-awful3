package homework.ch11_13.p3;

import java.util.ArrayList;
import java.util.List;

public class CourseTest {
    /**
     * 程序入口函数 在这里实例化教师对象、课程对象。同时实例化多个学生对象向课程注册。 需要创建一个 Course 数组，包含至少二门课程，每门课程至少注册三名学生。最
     * 后打印出每门课程的详细信息。 同时测试 Person、Student、Faculty、Course 的深拷贝
     * 功能，深拷贝测试包括： 克隆出来的对象和源对象内容相等； 克隆出来的对象和源对
     * 象所有引用类型数据成员指向的是不同对象。
     *
     * @param args
     * @throws CloneNotSupportedException
     */
    public static void main(String args[]) throws CloneNotSupportedException{
        Student student1=new Student("学生1",18,20,"CS",null);
        Student student2=new Student("学生2",19,30,"ME","K15475");
        Student student3=new Student("学生3",15,40,"QJ","K13522");
        Student student4=new Student("学生4",10,50,"WK","K58522");

        Faculty teacher1=new Faculty("王老师",30,15,"教授","123456@qq.com");
        Faculty teacher2=new Faculty("李老师",40,20,"副教授","654987@qq.com");

        Course[] courses=new Course[2];
        courses[0]=new Course("课程1",teacher1);
        courses[1]=new Course("课程2",teacher2);
        courses[0].register(student1);
        courses[0].register(student2);
        courses[0].register(student3);
        courses[1].register(student2);
        courses[1].register(student3);
        courses[1].register(student4);
        System.out.println(courses[0]);
        System.out.println(courses[1]);

        // 测试深拷贝
        Person p=null;
        Faculty s=teacher1;
        Faculty cloned=(Faculty) s.clone();
        assert s.equals(cloned) && s.getName() != cloned.getName() && s.getTitle() != cloned.getTitle() && s.getEmail() != cloned.getEmail();
    }
}
