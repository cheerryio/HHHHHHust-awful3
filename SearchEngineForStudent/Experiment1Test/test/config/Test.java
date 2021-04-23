package config;

import config.Config;

public class Test {
    public static void main(String[] args){
        System.out.println(Config.class.getResource("").getPath());
    }
}