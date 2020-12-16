import React, { FC, memo } from 'react';
import { Grid, IconButton } from '@material-ui/core';
import useStyles from '../../styles/digitBoard';
import { NumberSvg } from '../SmallComponents/NumberSvg';
export interface localProps {
    numbers: number[][];
    onMouseEnter: Function;
    onClick: Function;
}

const Digits: FC<localProps> = memo(({ numbers, onMouseEnter, onClick }) => {
    const classes = useStyles();

    return (
        <Grid container className={classes.digitsContainerLine}>
            {numbers.map((nums, index) => (
                <Grid key={`DigitBoardLine${index}`} container item spacing={0}>
                    {nums.map((num, index) => (
                        <Grid key={`DigitBoard${num}`} item spacing={0}>
                            <IconButton
                                className={classes.digitButtonContainer}
                                onMouseEnter={() => {
                                    onMouseEnter(num);
                                }}
                                onClick={() => {
                                    onClick(num);
                                }}>
                                <NumberSvg num={num} SvgProp={{ className: classes.digitButton }}></NumberSvg>
                            </IconButton>
                        </Grid>
                    ))}
                </Grid>
            ))}
        </Grid>
    );
});

export default Digits;
