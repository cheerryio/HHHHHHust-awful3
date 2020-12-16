import React, { FC, memo, useMemo, useContext, useCallback } from 'react';
import { Grid } from '@material-ui/core';

import useStyles from '../../styles/playBoard';

import { sudokuValue } from '../../types';
import { Props } from '../../containers/PlayBoard';
import NumberBlock from './NumberBlock';
import { ThemeContext } from '../../styles/withRoot';
import { digitsKeyMap, createDigitsHandlers } from '../../consts/hotKeys';
import { HotKeys } from 'react-hotkeys';

const PlayBoard: FC<Props> = memo(
    ({
        values,
        initValues,
        point,
        blockHighlight,
        playRound,
        placeValue,
        showUnchangeable,
        conflictValues,
        showConflict,
        showOptionNumber,
        toggleDigitBoardAction,
        clearBlockHighlightAction,
        chooseDigitStartAction,
        chooseDigitAction,
        playRoundForwardAction,
        blockHighlightAction,
    }) => {
        const classes = useStyles();
        const { darkMode } = useContext(ThemeContext);

        const handleChooseDigitHotKeys = useCallback(
            (value: sudokuValue) => {
                if (initValues[point.x][point.y] !== null) return;
                chooseDigitAction({ x: point.x, y: point.y, value });
                playRoundForwardAction({
                    x: point.x,
                    y: point.y,
                    from: point.value,
                    to: value,
                });
            },
            [point, initValues, chooseDigitAction, playRoundForwardAction],
        );

        let handlers = createDigitsHandlers(handleChooseDigitHotKeys);

        /**
         * 如果不是不可变的数字，当placeValue == null，点击不会对block中数字进行影响，点击应当拉起DigitBoard
         * 如果数字是1-9，对对应block数字设置为1-9
         * 如果数字是-1，代表清空block为空
         */
        const handleBlockClick = useCallback(
            (line: number, column: number, value: sudokuValue) => {
                if (initValues[line][column] === null) {
                    if (placeValue !== null) {
                        chooseDigitAction({ x: line, y: column, value: placeValue === -1 ? null : placeValue });
                        playRoundForwardAction({
                            x: line,
                            y: column,
                            from: value,
                            to: placeValue,
                        });
                    } else {
                        toggleDigitBoardAction();
                    }
                }
            },
            [placeValue, initValues, chooseDigitAction, playRoundForwardAction, toggleDigitBoardAction],
        );

        const handleOptionClick = useCallback(
            (line: number, column: number, value: sudokuValue) => {
                chooseDigitAction({ x: line, y: column, value });
                playRoundForwardAction({
                    x: line,
                    y: column,
                    from: null,
                    to: value,
                });
            },
            [chooseDigitAction, playRoundForwardAction],
        );

        /**
         * 将1x9数组展开成一横行9个数独block
         * rely props:playRound,values,showOptionNumber,initValues,showUnchangeable,showConflict,conflictValues,blockHighlight
         * @param nums 1x9 matrix
         *
         * @returns JSX.ELEMENT array
         */
        const mapPlayBoardBlockLine = useCallback(
            (nums: sudokuValue[], line: number) => {
                return (
                    <Grid key={`PlayBoard${line}`} container item spacing={0}>
                        {nums.map((num: sudokuValue, column: number) => (
                            <NumberBlock
                                key={`NumberBlockLine${line}Column${column}`}
                                line={line}
                                column={column}
                                values={values}
                                num={num}
                                playRound={playRound}
                                placeValue={placeValue}
                                initValue={initValues[line][column]}
                                conflictValue={conflictValues[line][column]}
                                showConflict={showConflict}
                                showOptionNumber={showOptionNumber}
                                showUnchangeable={showUnchangeable}
                                blockhighlight={blockHighlight[line][column] === 1 ? true : false}
                                blockOnMouseEnter={(line: number, column: number, num: sudokuValue) => {
                                    chooseDigitStartAction({ x: line, y: column, value: num });
                                    blockHighlightAction(num);
                                }}
                                blockOnClick={handleBlockClick}
                                blockOnMouseLeave={clearBlockHighlightAction}
                                optionOnMouseEnter={blockHighlightAction}
                                optionOnClick={handleOptionClick}
                                optionOnMouseLeave={clearBlockHighlightAction}
                            />
                        ))}
                    </Grid>
                );
            },
            [
                values,
                playRound,
                placeValue,
                initValues,
                conflictValues,
                showConflict,
                showOptionNumber,
                showUnchangeable,
                blockHighlight,
                handleBlockClick,
                clearBlockHighlightAction,
                blockHighlightAction,
                handleOptionClick,
                clearBlockHighlightAction,
                chooseDigitStartAction,
            ],
        );

        return (
            <>
                {useMemo(() => {
                    return (
                        <HotKeys keyMap={digitsKeyMap} handlers={handlers} allowChanges>
                            <div className={classes.playBoardContainer} onMouseLeave={clearBlockHighlightAction}>
                                <Grid container spacing={0}>
                                    {values.map(mapPlayBoardBlockLine)}
                                </Grid>
                            </div>
                        </HotKeys>
                    );
                }, [
                    placeValue,
                    handlers,
                    classes.playBoardContainer,
                    clearBlockHighlightAction,
                    mapPlayBoardBlockLine,
                ])}
            </>
        );
    },
);

export default PlayBoard;
