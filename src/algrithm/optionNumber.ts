import { sudokuValue } from '../types';

/**
 * 等待完成
 * @param value 棋盘数据
 * @param y 判断位置的横坐标（从0-8）
 * @param x 判断位置的纵坐标（从0-8）
 *
 * @returns 棋盘中当前位置能填入什么
 */
const a = null;
export function optionNumber(value: sudokuValue[][], x: number, y: number): sudokuValue[] {
    let OptionNumber: sudokuValue[] = [1, 2, 3, 4, 5, 6, 7, 8, 9];
    let row: number, column: number, num: number;
    for (row = 0; row < 9; row++) {
        if (value[row][y] !== a) OptionNumber[(value[row][y] as number) - 1] = null;
    }
    for (column = 0; column < 9; column++) {
        if (value[x][column] !== a) OptionNumber[(value[x][column] as number) - 1] = null;
    }
    for (num = 0; num < 9; num++) {
        if (value[Math.floor(x / 3) * 3 + (num % 3)][Math.floor(y / 3) * 3 + Math.floor(num / 3)] !== a)
            OptionNumber[
                (value[Math.floor(x / 3) * 3 + (num % 3)][Math.floor(y / 3) * 3 + Math.floor(num / 3)] as number) - 1
            ] = null;
    }
    return OptionNumber;
}

export default optionNumber;
