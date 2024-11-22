#include "listitembase.h"

ListItemBase::ListItemBase(QWidget *parent): QWidget(parent)
{

}

void ListItemBase::setItemType(ListItemType itemType)
{
    _itemType = itemType;
}

ListItemType ListItemBase::getItemType()
{
    return _itemType;
}
