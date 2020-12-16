import React, { FC, memo, useState, useMemo, useEffect, useCallback } from 'react';
import { useSnackbar } from 'notistack';

import Grid from '@material-ui/core/Grid';
import IconButton from '@material-ui/core/IconButton';
import Switch from '@material-ui/core/Switch';
import MenuItem from '@material-ui/core/MenuItem';
import TextField from '@material-ui/core/TextField';
import FormControl from '@material-ui/core/FormControl';
import FormGroup from '@material-ui/core/FormGroup';
import FormControlLabel from '@material-ui/core/FormControlLabel';

import classNames from 'classnames';
import { numberIcons } from '../../consts/elements';
import useStyles from '../../styles/playHelper';

import { Props } from '../../containers/PlayHelper';
import TipButton from '../SmallComponents/TipButton';

import { Undo, Refresh, VisibilityOutlined, VisibilityOffOutlined } from '@material-ui/icons';
import NumberSvg from '../SmallComponents/NumberSvg';

const PlayHelper: FC<Props> = memo(
    ({
        complete,
        blockHighlightAction,
        setPlaceValueAction,
        clearPlaceValueAction,
        toggleShowUnchangeableAction,
        updateSudokuAction,
        playRoundBackwardAction,
        clearBlockHighlightAction,
        toggleShowConflictAction,
        toggleShowOptionNumberAction,
        setLevelAction,
    }) => {
        const classes = useStyles();
        const { enqueueSnackbar, closeSnackbar } = useSnackbar(); // eslint-disable-line

        const [completeMessageShowed, setCompleteMessageshowed] = useState(
            false,
        ); /** 数独正确完成，是否展示过成功信息*/
        /** 使用enqueueSnackbar弹出消息框，展示数独成功完成，并将已经展示状态为设置为true */
        useEffect(() => {
            if (completeMessageShowed === false && complete === true) {
                enqueueSnackbar('Sudoku Complete!', { variant: 'info' });
                setCompleteMessageshowed(true);
            }
        }, [complete, completeMessageShowed, enqueueSnackbar]);

        const [highlightLoc, setHighlightLoc] = useState(
            0,
        ); /** 1-9哪个数字需要高亮，0代表没有数字需要高亮,-1表示删除点击数字*/

        /** 按钮文本，可更改*/
        const showConflictTexts = ['取消显示冲突', '显示冲突'];

        const [showOptionNumberIcon, setShowOptionNumberIcon] = useState(false); //true 为可见 false 为不可见
        const [showUnchangeableSwitch, setShowUnchangeableSwitch] = useState(true);
        const [showUnchangeableText, setShowUnchangeableText] = useState('取消不可变显示');
        const [showConflictSwitch, setShowConflictSwitch] = useState(true);
        const [showConflictText, setShowConflictText] = useState(showConflictTexts[0]);

        /** toggle button text*/
        const toggleShowUnchangeable = useCallback(() => {
            const showUnchangeableTexts = ['取消不可变显示', '显示不可变显示'];
            showUnchangeableText === showUnchangeableTexts[0]
                ? setShowUnchangeableText(showUnchangeableTexts[1])
                : setShowUnchangeableText(showUnchangeableTexts[0]);
            setShowUnchangeableSwitch((prev) => !prev);
        }, [showUnchangeableText]);

        /** toggle button text*/
        const toggleShowConflict = () => {
            showConflictText === showConflictTexts[0]
                ? setShowConflictText(showConflictTexts[1])
                : setShowConflictText(showConflictTexts[0]);
            setShowConflictSwitch((prev) => !prev);
        };

        /** toggle button text*/
        const toggleShowOptionNumberIcon = () => {
            setShowOptionNumberIcon((prev) => !prev);
        };

        /**
         * map to show numberIcon 1-9
         *
         * rely props:highligLoc
         *
         * @param value map keys
         * @param index from 0
         *
         * @returns JSX.Element
         */
        const mapNumberIcon = useCallback(
            (value: number, index: number) => (
                <Grid key={`value${value}`} item className={classes.numberIconNormal}>
                    <IconButton
                        onMouseEnter={() => {
                            blockHighlightAction(value);
                        }}
                        onClick={() => {
                            if (highlightLoc === value) {
                                clearPlaceValueAction();
                                setHighlightLoc(0);
                            } else {
                                setPlaceValueAction(value);
                                setHighlightLoc(value);
                            }
                        }}
                        onMouseLeave={clearBlockHighlightAction}
                        className={classNames(classes.numberIconNormal)}>
                        <NumberSvg
                            num={value}
                            SvgProp={{
                                className: classNames(classes.numberIconNormal, {
                                    [classes.hightLight]: highlightLoc === value,
                                }),
                            }}></NumberSvg>
                    </IconButton>
                </Grid>
            ),
            [
                blockHighlightAction,
                classes.hightLight,
                classes.numberIconNormal,
                clearBlockHighlightAction,
                clearPlaceValueAction,
                highlightLoc,
                setPlaceValueAction,
            ],
        );

        return (
            <React.Fragment>
                <Grid container className={classNames(classes.numberContainerNormal, {})}>
                    {useMemo(() => Array.from(numberIcons.keys()).map(mapNumberIcon), [mapNumberIcon])}
                </Grid>
                <Grid container className={classes.buttonContainers}>
                    {/** 撤销填写数独操作*/}
                    {useMemo(
                        () => (
                            <TipButton
                                TooltipProp={{ title: '撤回上一步', children: <></>, arrow: true }}
                                IconButtonProp={{
                                    onClick: playRoundBackwardAction,
                                    className: classNames(classes.iconButtonContainer, {}),
                                }}>
                                <Undo className={classNames(classes.iconButtonIcon, {})} />
                            </TipButton>
                        ),
                        [classes.iconButtonContainer, classes.iconButtonIcon, playRoundBackwardAction],
                    )}
                    {/** 开始新一局数独，调用updateSudoku*/}
                    {useMemo(
                        () => (
                            <TipButton
                                TooltipProp={{ title: '开始新一局', children: <></>, arrow: true }}
                                IconButtonProp={{
                                    onClick: () => {
                                        setCompleteMessageshowed(
                                            false,
                                        ); /** 开始了新一局游戏，该局游戏还没有显示过success*/
                                        updateSudokuAction();
                                    },
                                    className: classNames(classes.iconButtonContainer, {}),
                                }}>
                                <Refresh className={classNames(classes.iconButtonIcon, {})} />
                            </TipButton>
                        ),
                        [classes.iconButtonContainer, classes.iconButtonIcon, updateSudokuAction],
                    )}
                    {/** 显示或者隐藏可选数字(optionNumber)*/}
                    {useMemo(
                        () => (
                            <TipButton
                                TooltipProp={{
                                    title: showOptionNumberIcon ? '显示可选数字' : '隐藏可选数字',
                                    children: <></>,
                                    arrow: true,
                                }}
                                IconButtonProp={{
                                    onClick: () => {
                                        toggleShowOptionNumberIcon();
                                        toggleShowOptionNumberAction();
                                    },
                                    className: classNames(classes.iconButtonContainer, {}),
                                }}>
                                <VisibilityOutlined
                                    className={classNames(classes.iconButtonIcon, {
                                        [classes.hideElement]: showOptionNumberIcon,
                                    })}
                                />
                                <VisibilityOffOutlined
                                    className={classNames(classes.iconButtonIcon, {
                                        [classes.hideElement]: !showOptionNumberIcon,
                                    })}
                                />
                            </TipButton>
                        ),
                        [
                            showOptionNumberIcon,
                            classes.hideElement,
                            classes.iconButtonContainer,
                            classes.iconButtonIcon,
                            toggleShowOptionNumberAction,
                        ],
                    )}

                    <TextField
                        select
                        label='难度'
                        defaultValue={0}
                        onChange={(event) => {
                            setLevelAction(event.target.value);
                        }}>
                        <MenuItem value={0}>0超级简单</MenuItem>
                        <MenuItem value={1}>1简单</MenuItem>
                        <MenuItem value={2}>2普通</MenuItem>
                        <MenuItem value={3}>3困难</MenuItem>
                    </TextField>
                </Grid>
                <FormControl component='fieldset'>
                    <FormGroup aria-label='position' row>
                        {useMemo(
                            () => (
                                <FormControlLabel
                                    value={'unchangeable'}
                                    control={
                                        <Switch
                                            checked={showUnchangeableSwitch}
                                            onChange={() => {
                                                toggleShowUnchangeable();
                                                toggleShowUnchangeableAction();
                                            }}
                                            color='primary'
                                        />
                                    }
                                    label={showUnchangeableText}
                                    labelPlacement={'bottom'}
                                />
                            ),
                            [
                                showUnchangeableText,
                                showUnchangeableSwitch,
                                toggleShowUnchangeable,
                                toggleShowUnchangeableAction,
                            ],
                        )}

                        {useMemo(
                            () => (
                                <FormControlLabel
                                    value={'conflict'}
                                    control={
                                        <Switch
                                            checked={showConflictSwitch}
                                            onChange={() => {
                                                toggleShowConflict();
                                                toggleShowConflictAction();
                                            }}
                                            color='primary'
                                        />
                                    }
                                    label={showConflictText}
                                    labelPlacement={'bottom'}
                                />
                            ),
                            [
                                showConflictText,
                                showOptionNumberIcon,
                                showConflictSwitch,
                                toggleShowConflict,
                                toggleShowConflictAction,
                            ],
                        )}
                    </FormGroup>
                </FormControl>
            </React.Fragment>
        );
    },
);
export default PlayHelper;
