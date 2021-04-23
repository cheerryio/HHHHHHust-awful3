public class Qbig5 {
    /**
     * 创建一个不规则二维数组
     * 第一行row列
     * 第二行row - 1列
     * ...
     * 最后一行1列
     * 数组元素值都为默认值
     * <p>
     * 例如：当row=5
     * 0 0 0 0 0
     * 0 0 0 0
     * 0 0 0
     * 0 0
     * 0
     *
     * @param row 行数
     * @return 创建好的不规则数组
     */
    public static int[][] createArray(int row) {
        if (row <= 0)
            return null;
        int curRow=row;
        int[][] myArray = new int[row][];
        for (int i = 0; i < row; i++) {
            myArray[i] = new int[curRow];
            curRow--;
        }
        return myArray;
    }

    /**
     * 逐行打印出二维数组，数组元素之间以空格分开
     *
     * @param a
     */
    public static void printArray(int[][] a) {
        for (int m = 0; m < a.length; m++) {
            for (int n = 0; n < a[m].length; n++) {
                System.out.print(a[m][n]);
                System.out.print(" ");
            }
            System.out.println("\n");
        }
    }

    public static void main(String[] args) {
        int[][] myArray = Qbig5.createArray(5);
        if (myArray != null)
            Qbig5.printArray(myArray);
    }
}
