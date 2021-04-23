import java.io.IOException;

public class Q16 {
    public void m1() throws IOException {
        try {
            throw new IOException();
        }
        catch (IOException e){

        }
    }

    public void m2() throws IOException{
        m1();
    }
}
