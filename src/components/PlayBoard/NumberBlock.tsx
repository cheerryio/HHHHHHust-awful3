import React, { FC, memo, useMemo } from 'react';
import useStyles from '../../styles/playBoard';
import classNames from 'classnames';
import { Grid, IconButton } from '@material-ui/core';
import { sudokuValue, conflictValue, PlaceValue } from '../../types';
import NumberIcon from './NumberIcon';
import optionNumber from '../../algrithm/optionNumber';
import NumberOption from './NumberOption';

export interface localProps {
    key: string;
    line: number;
    column: number;
    num: sudokuValue;
    playRound: number;
    placeValue: PlaceValue;
    initValue: sudokuValue;
    conflictValue: conflictValue;
    showOptionNumber: boolean;
    showConflict: boolean;
    showUnchangeable: boolean;
    blockhighlight: boolean;
    blockOnMouseEnter: Function;
    blockOnClick: Function;
    blockOnMouseLeave: Function;
    values: sudokuValue[][];
    optionOnMouseEnter: Function;
    optionOnClick: Function;
    optionOnMouseLeave: Function;
}

function compareNumberBlock(prevProps: any, nextProps: any) {
    /**
     * 注意有一种情况，当initValue=null,showOptionNumber变为true或者变为false
     *
     * 让组件onBlockClick函数使用useCallBack调用PlayBoard组件里面的placeValue全局变量。在PlayBoard里面该函数更新
     * 了以后，需要让此组件同样跟去placeValue去更新一下才行！这样里面的函数才能同样被更新！
     */
    return (
        prevProps.num === nextProps.num &&
        prevProps.initValue === nextProps.initValue &&
        prevProps.blockhighlight === nextProps.blockhighlight &&
        prevProps.showOptionNumber === nextProps.showOptionNumber &&
        prevProps.showConflict === nextProps.showConflict &&
        prevProps.showUnchangeable === nextProps.showUnchangeable &&
        prevProps.conflictValue === nextProps.conflictValue &&
        prevProps.playRound === nextProps.playRound &&
        prevProps.placeValue === nextProps.placeValue
    );
}

const NumberBlock: FC<localProps> = memo(
    ({
        line,
        column,
        num,
        initValue,
        playRound,
        placeValue,
        showOptionNumber,
        showConflict,
        showUnchangeable,
        conflictValue,
        blockhighlight,
        blockOnMouseEnter,
        blockOnClick,
        blockOnMouseLeave,
        values,
        optionOnMouseEnter,
        optionOnClick,
        optionOnMouseLeave,
    }) => {
        const classes = useStyles();
        const optNumber: sudokuValue[] = optionNumber(values, line, column);
        return (
            <div className={classes.PlayBoardLine} key={`PlayBoardLine${line}Block${column}`}>
                <Grid
                    item
                    container
                    className={classNames({
                        [classes.bottomPaddingBorder]: line === 2 || line === 5,
                        //[classes.topPadding]: !(line % 3),
                        [classes.rightPaddingBorder]: column === 2 || column === 5,
                        //[classes.leftPadding]: !(column % 3),
                        [classes.splitBorder]: true,
                    })}>
                    {useMemo(
                        () => (
                            <IconButton
                                className={classNames(classes.playBoardBlockContainer, {
                                    [classes.hidenullIcon]: showOptionNumber === true && num === null,
                                })}
                                onMouseEnter={() => {
                                    blockOnMouseEnter(line, column, num);
                                }}
                                onClick={() => {
                                    if (initValue === null) blockOnClick(line, column, num);
                                }}
                                onMouseLeave={() => {
                                    blockOnMouseLeave();
                                }}
                                disabled={showOptionNumber === true && num === null}>
                                <NumberIcon
                                    num={num}
                                    initNum={initValue}
                                    showUnchangeable={showUnchangeable}
                                    /** 最后一个条件num === 1 | 2 ... 可以去掉，作为优化*/
                                    className={classNames(classes.numberIconNormal, {
                                        [classes.hightLight]: blockhighlight,
                                        [classes.conflictNumber]: showConflict && conflictValue,
                                    })}
                                />
                            </IconButton>
                        ),
                        [
                            num,
                            initValue,
                            showUnchangeable,
                            showOptionNumber,
                            classes.numberIconNormal,
                            blockhighlight,
                            showConflict,
                            conflictValue,
                            blockOnMouseEnter,
                            blockOnClick,
                            blockOnMouseLeave,
                            optionOnMouseEnter,
                            optionOnClick,
                            optionOnMouseLeave,
                        ],
                    )}

                    {useMemo(() => {
                        if (num === null && showOptionNumber === true && initValue === null) {
                            return (
                                <Grid
                                    container
                                    item
                                    className={classNames(classes.optionNumberBlock, {
                                        [classes.optionalNumberTopPadding]: !(line % 3),
                                    })}>
                                    {optNumber.map((num, c) => (
                                        <NumberOption
                                            key={`line${line}column${column}index${c}optnum${num}`}
                                            num={num}
                                            onMouseEnter={() => {
                                                optionOnMouseEnter(num);
                                            }}
                                            onClick={() => {
                                                optionOnClick(line, column, num);
                                            }}
                                            onMouseLeave={() => {
                                                optionOnMouseLeave();
                                            }}
                                        />
                                    ))}
                                </Grid>
                            );
                        }
                    }, [
                        values,
                        num,
                        showOptionNumber,
                        initValue,
                        playRound,
                        classes.optionNumberBlock,
                        classes.optionalNumberTopPadding,
                        column,
                        line,
                        optionOnClick,
                        optionOnMouseEnter,
                        optionOnMouseLeave,
                    ])}
                </Grid>
            </div>
        );
    },
);

export default NumberBlock;
