import { conflictValue } from '../types';

/**
 * 检测数独盘中的冲突
 *
 * @param values 数独盘 9x9 matrix
 *
 * @returns conflict:是否冲突 true-冲突,false-不冲突
 *          complete:是否过关，即数独被正确完成
 *          conflict:冲突点 数字大小代表第几个数字冲突 数字从1-9,null代表此格不与任何格子冲突
 */

const a: conflictValue = null;
let conflictValues: conflictValue[][] = [
    [a, a, a, a, a, a, a, a, a],
    [a, a, a, a, a, a, a, a, a],
    [a, a, a, a, a, a, a, a, a],
    [a, a, a, a, a, a, a, a, a],
    [a, a, a, a, a, a, a, a, a],
    [a, a, a, a, a, a, a, a, a],
    [a, a, a, a, a, a, a, a, a],
    [a, a, a, a, a, a, a, a, a],
    [a, a, a, a, a, a, a, a, a],
];
let conflict: boolean = false;
let complete: boolean = true;
export function conflictDetect(values: conflictValue[][]) {
    for (let i = 0; i < 9; i++) {
        for (let j = 0; j < 9; j++) {
            conflictValues[i][j] = a;
        }
    }
    conflict = false;
    complete = true;
    isConflict(values);
    isSuccess(values);
    return {
        conflict,
        complete,
        conflictValues,
    };
}

function isConflict(values: conflictValue[][]) {
    let row: number, column: number, num: number, x: number, y: number;
    //按照行遍历是否冲突
    for (row = 0; row < 9; row++) {
        for (column = 0; column < 9; column++) {
            for (num = column + 1; num < 9; num++) {
                if (values[row][column] === values[row][num]) {
                    conflictValues[row][column] = values[row][column];
                    conflictValues[row][num] = values[row][num];
                    conflict = true;
                }
            }
        }
    }
    for (column = 0; column < 9; column++) {
        for (row = 0; row < 9; row++) {
            for (num = row + 1; num < 9; num++) {
                if (values[row][column] === values[num][column]) {
                    conflictValues[row][column] = values[row][column];
                    conflictValues[num][column] = values[num][column];
                    conflict = true;
                }
            }
        }
    }
    for (num = 0; num < 9; num++) {
        for (x = 0; x < 9; x++) {
            for (y = x + 1; y < 9; y++) {
                if (
                    values[ROW(num) * 3 + ROW(x)][COLUMN(num) * 3 + COLUMN(x)] ===
                    values[ROW(num) * 3 + ROW(y)][COLUMN(num) * 3 + COLUMN(y)]
                ) {
                    conflictValues[ROW(num) * 3 + ROW(x)][COLUMN(num) * 3 + COLUMN(x)] =
                        values[ROW(num) * 3 + ROW(x)][COLUMN(num) * 3 + COLUMN(x)];
                    conflictValues[ROW(num) * 3 + ROW(y)][COLUMN(num) * 3 + COLUMN(y)] =
                        values[ROW(num) * 3 + ROW(x)][COLUMN(num) * 3 + COLUMN(x)];
                    conflict = true;
                }
            }
        }
    }
    return conflict;
}

function ROW(x: number): number {
    return Math.floor(x / 3);
}

function COLUMN(x: number): number {
    return x % 3;
}
function isSuccess(values: conflictValue[][]): boolean {
    // console.log("in IsSuccess");
    let row: number, column: number;
    if (conflict) complete = false;
    else {
        for (row = 0; row < 9; row++) {
            for (column = 0; column < 9; column++) {
                if (values[row][column] === a) complete = false;
            }
        }
    }
    // console.log(complete);
    return complete;
}

export default conflictDetect;
