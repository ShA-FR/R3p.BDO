using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using R3p.bdo.Memory;

namespace R3p.bdo.GameInternals.Structs.ItemMarket
{
    public class MarketList : MemoryObject
    {
        public MarketList()
        {
            Address = Offsets._marketBase;
        }

        //private int x0000_ListCount => ReadInt32(0x50);
        private long x0008_ItemListStart => ReadPointer8b(0x00);
        private long x0010_ItemListEnd => ReadPointer8b(0x08);

        private long RegistredItemList_Start => ReadPointer8b(0x48);
        private long RegistredItemList_End => ReadPointer8b(0x50);

        public int RegistredItemCount => (int)(RegistredItemList_End - RegistredItemList_Start)/0xD8;

        public Dictionary<int, List<MarketItemData>> List => GetItemList();

        private Dictionary<int, List<MarketItemData>> _lBuffer;
        private Dictionary<int, List<MarketItemData>> GetItemList()
        {
            if(_lBuffer != null)
                return _lBuffer;

            Dictionary<int, List<MarketItemData>> itemList = new Dictionary<int, List<MarketItemData>>();

            var start = x0008_ItemListStart;
            var end = x0010_ItemListEnd;
            int maxSize = (int)(end - start) / 0x08;
            
            for (int i = 0; i < maxSize; i+= 8)
            {
                MarketItemData curNode = new MarketItemData(ReadInt64(start + i));

                int itemId = (int)curNode.ItemData.ItemIndex;

                if (!itemList.ContainsKey(itemId))
                {
                    itemList.Add(itemId, new List<MarketItemData>() {curNode});
                }
                else
                {
                    itemList[itemId].Add(curNode);
                }
            }

            if (_lBuffer == null)
                _lBuffer = itemList;

            return itemList;
        }
    }
}
