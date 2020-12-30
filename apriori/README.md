# Apriori

## 关联关系挖掘
由购买了n个物品推出购买了一个物品

## 支持度和置信度(support and confidence)
支持度衡量一个项集在baskets中的频繁程度

置信度衡量在购买了n个物品后，i物品出现的概率

## apriori
由备选集C生成频繁项集L，再由L生成C。在L生成C的过程中，假设L是频繁n项集，要生成的C是n+1项的备选集。要验证C中每一项的每个n项子集是否是频繁n项，如果不是那么这个备选n+1项集就一定不是频繁n+1项

## pcy
在生成L1之前，将源数据集中每一个pair哈希到桶中，对桶中计数大于support的对应bitmap位置1。生成C2时，验证pair哈希到的bitmap位是否为1

## multi_stage
在pcy的基础上，对生成的L1的每个pair生成bitmap。在生成C2时，还要验证pair哈希到的bitmap位是否为1

## multi_hash
在pcy实现的基础上，用多个不同的哈希函数生成多个bitmap。生成C2时，验证pair在多个bitmap中哈希到的位是否为1

## 关联规则
n项关联到1项。由Ln-1和Ln共同生成。当关联置信度>预设值，该关联规则有效