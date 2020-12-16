import React, { FC, memo } from 'react';
import { Tooltip, IconButton, TooltipProps, IconButtonProps } from '@material-ui/core';
/**
 * 包装了 Tooltip和 IconButton的一个组件，将属性传下去。为了减少缩进？
 *  <Tooltip {...TooltipProp}>
 *      <IconButton {...IconButtonProp} >
 *          {children}
 *      </IconButton>
 *  </Tooltip>
 * @property TooltipProps: 放到Tooltip中的 Props
 * @property IconButtonProps: 放到IconButton中的 Props
 */
export const TipButton: FC<{ TooltipProp: TooltipProps; IconButtonProp: IconButtonProps }> = memo(
    ({ TooltipProp, IconButtonProp, children }) => {
        return (
            <Tooltip {...TooltipProp}>
                <IconButton {...IconButtonProp}>{children}</IconButton>
            </Tooltip>
        );
    },
);

export default TipButton;
