package homework;

import java.lang.Math;

/**
 *
 * 2：假设一个车牌号码由三个大写字母和后面的四个数字组成。编写一个程序. 生
 * 成5个不重复的车牌号码。
 *
 */

public class Chapter45_licenseplate {
    public static void main(String[] args){
        String[] licensePlates=new String[5];
        for(int i=0;i<licensePlates.length;){
            char randomLetter;
            int randomDigit;
            StringBuffer licensePlate=new StringBuffer("");
            for(int m=0;m<3;m++){
                randomLetter=(char)('A'+(int)(Math.random()*('Z'-'A')));
                licensePlate.append(randomLetter);
            }
            for(int m=0;m<4;m++){
                randomDigit=(int)(Math.random()*(9+1));
                licensePlate.append(randomDigit);
            }
            if(i==0){
                licensePlates[i]=licensePlate.toString();
                i++;
                continue;
            }
            for(int m=0;m<i;m++){
                String s1=licensePlates[m];
                String s2=licensePlate.toString();
                if(s1.equals(s2)){
                    break;
                }else{
                }
            }
            licensePlates[i]=licensePlate.toString();
            i++;
        }
        for(String licensePlate:licensePlates){
            System.out.println(licensePlate);
        }
    }
}
