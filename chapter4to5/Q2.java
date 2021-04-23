public class Q2 {
    public static void main(String[] args){
        StringBuffer s1=new StringBuffer("Java");
        StringBuffer s2=new StringBuffer("HTML");
        StringBuffer s=new StringBuffer("Welcome to JAVA");

        // System.out.println("s1.append(s2): "+s1.append(s2));
        // System.out.println("s1.insert(2,\"is fun\"): "+s1.insert(2,"is fun"));
        // System.out.println("s1.delete(1,3): "+s1.delete(1,3));
        // System.out.println("s1.replace(1,3,\"Computer\"): "+s1.replace(1,3,"Computer"));
        s.setLength(0);
        System.out.println("s: "+s);
    }
}
