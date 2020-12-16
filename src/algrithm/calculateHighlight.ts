import { sudokuValue } from '../types';

/**
 * calculateHighlight 根据传入value计算矩阵中所有值等于value的格子
 *
 * @param values current 9x9 board value matrix
 * @param value value given to check dulplicate
 *
 * @returns blockHighlight matrix 值等于value的格子为1， 值不等于value的格子为0
 */
export function calculateHighlight(values: sudokuValue[][], value: sudokuValue) {
    let blockHighlight: number[][] = [[], [], [], [], [], [], [], [], []];
    for (var m: number = 0; m < 9; m++)
        for (var n: number = 0; n < 9; n++)
            if (value != null && value === values[m][n]) blockHighlight[m][n] = 1;
            else blockHighlight[m][n] = 0;
    return blockHighlight;
}

export default calculateHighlight;
