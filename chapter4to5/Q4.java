public class Q4{
    public static void main(String[] args){
        String s1="Welcome";
        String s2="Wel";
        String s3="come";

        System.out.println(s1==(s2+s3).intern());
    }
}
