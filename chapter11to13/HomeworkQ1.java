import java.util.ArrayList;
import java.util.List;

/**
 * 任务接口
 */
interface Task {
    /**
     * 执行具体任务的接口方法
     */
    public abstract void execute();
}

/**
 * 任务服务接口
 */
interface TaskService {
    /**
     * 执行任务接口列表中的每个任务
     */
    public void executeTasks();
    /**
     * 添加任务
     * @param t 新添加的任务
     */
    public void addTask(Task t);
}

class Task1 implements Task {
    @Override
    public void execute() {
        System.out.println("execute task1");
    }
}
class Task2 implements Task {
    @Override
    public void execute() {
        System.out.println("execute task2");
    }
}
class Task3 implements Task {
    @Override
    public void execute() {
        System.out.println("execute task3");
    }
}

class TaskServiceImpl implements TaskService {

    private List<Task> tasks= new ArrayList<Task>();

    @Override
    public void executeTasks() {
        for(Task task:this.tasks){
            task.execute();
        }
    }

    @Override
    public void addTask(Task t) {
        this.tasks.add(t);
    }
}

public class HomeworkQ1 {
    public static void main(String args[]){
        Task task1=new Task1();
        Task task2=new Task2();
        Task task3=new Task3();
        TaskService taskService=new TaskServiceImpl();
        taskService.addTask(task1);
        taskService.addTask(task2);
        taskService.addTask(task3);
        taskService.executeTasks();
    }
}



