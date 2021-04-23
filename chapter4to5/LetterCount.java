import java.util.Scanner;

/**
 *
 * 1：编写程序，从控制台或对话框任意输入一个英文字符串，统计字符串中每个英文字母出现的次数并输出到控制台（大小写不敏感）。
 *
 * A-Z 65-90
 * a-z 97-122
 */

public class LetterCount {
    public static void main(String[] args){
        Scanner scanner=new Scanner(System.in);
        String s=scanner.nextLine();
        int[] counts=new int['Z'-'A'+1+'z'-'a'+1];
        char[] letters=s.toCharArray();
        for(char letter:letters){
            if(letter<='Z' && letter>='A'){
                counts[letter-'A']+=1;
            }else if(letter<='z' && letter>='a'){
                counts['Z'-'A'+1+letter-'a']+=1;
            }else{
                // 输入不是英文字母 不处理
            }
        }
        for(int i=0;i<'Z'-'A'+1+'z'-'a'+1;i++){
            if(i<'Z'-'A'+1){
                System.out.println((char)('A'+i)+": "+counts[i]);
            }else{
                System.out.println((char)('a'+i-('Z'-'A'+1))+": "+counts[i]);

            }
        }
    }
}
