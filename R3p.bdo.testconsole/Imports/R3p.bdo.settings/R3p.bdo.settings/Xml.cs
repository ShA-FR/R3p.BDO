using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using R3p.bdo.GameExternals.Enums;
using R3p.bdo.GameExternals.Structs.AutoItemBuy;
using R3p.bdo.GameExternals.Structs.AutoItemRegister;
using R3p.bdo.GameExternals.Structs.AutoProcessing;
using R3p.bdo.GameExternals.Structs.Overlay;
using R3p.bdo.GameInternals.Structs.SystemVariables;

namespace R3p.bdo.settings
{
    public class Xml
    {

        #region public properties

        public static XmlDocument XmlDoc { get; set; }
        public static XmlNode XmlRoot { get; set; }

        #endregion public properties


        public static int[] ParseIdList(string idList)
        {
            List<int> ids = new List<int>();
            foreach (string idString in idList.Split(new char[] { ',' }).ToList())
            {
                int.TryParse(idString, out int id);
                if (id == 0) { continue; }
                ids.Add(id);
            }
            return ids.ToArray();
        }

        public static string BoolToIntToString(bool value)
        {
            return Convert.ToInt32(value).ToString();
        }

        /// <summary>
        /// Check if value it's null and throw ArgumentNullException
        /// </summary>
        /// <param name="value"></param>
        public static bool ThrowIfNull(string value)
        {
            if (value == null)
            {
                throw new ArgumentNullException(String.Format("This argument must'n be null {0}", value));
            }
            return false;
        }

        /// <summary>
        /// BUG: Can't create comment at parent root node
        /// </summary>
        /// <param name="keyName">name of xml tag</param>
        /// <param name="parentNode">instance of XmlNode for parent node</param>
        /// <param name="attributes">Dictionary of key+value for xml tag attributes</param>
        /// <param name="comment">if enabled create comment tag, false by default</param>
        /// <returns></returns>
        private static XmlNode XmlCreateKey(string keyName, XmlNode parentNode=null, Dictionary<string, string> attributes=null, bool comment=false)
        {
            XmlNode element = null;

            // ThrowIfNull(keyName);
            // create element
            if (comment)
            {
                element = XmlDoc.CreateComment(keyName);
            }
            else
            {
                element = XmlDoc.CreateElement(keyName);
            }
            // create attributes for element
            if (attributes != null)
            {
                foreach (var attr in attributes)
                {
                    XmlAttribute attribute = XmlDoc.CreateAttribute(attr.Key);
                    attribute.Value = attr.Value;
                    element.Attributes.Append(attribute);
                }
            }
            // add if parent
            if (parentNode != null)
            {
                parentNode.AppendChild(element);
            }
            return element;
        }

        /// <summary>
        /// Save settings to XML file
        /// </summary>
        /// <returns>if save process was success or not</returns>
        public static bool SaveSettings(string rootNodeName = "Settings")
        {
            ThrowIfNull(rootNodeName);
            // minimal objects to fill up new settings.xml file
            XmlDoc = new XmlDocument();
            XmlRoot = XmlDoc.CreateElement(rootNodeName);
            XmlDoc.AppendChild(XmlRoot);
            XmlCreateKey(String.Join("\t", Enum.GetNames(typeof(VirtualKeyCode))), parentNode: XmlRoot, comment: true);
            XmlCreateKey("HotKeys", XmlRoot, Settings.HotKeys.List);
            XmlCreateKey("AutoFish", XmlRoot, Settings.AutoFish.List);
            XmlCreateKey("AutoRestore", XmlRoot, Settings.AutoRestore.List);
            XmlCreateKey("UIHack", XmlRoot, Settings.UIHack.List);
            XmlNode nodeSpeedHack = XmlCreateKey("SpeedHack", XmlRoot, Settings.SpeedHack.List);
            XmlCreateKey("Horse", nodeSpeedHack, Settings.SpeedHack.Horse.List);
            XmlCreateKey("Ship", nodeSpeedHack, Settings.SpeedHack.Ship.List);
            XmlCreateKey("Player", nodeSpeedHack, Settings.SpeedHack.Player.List);
            XmlCreateKey("AutoPotion", XmlRoot, Settings.AutoPotion.List);
            XmlNode nodeAutoItemRegister = XmlCreateKey("AutoItemRegister", XmlRoot, Settings.AutoItemRegister.List);
            XmlNode nodeAutoItemRegisterItems = XmlCreateKey("Items", nodeAutoItemRegister);
            foreach (var itemEntry in Settings.AutoItemRegister.Items)
            {
                Dictionary<string, string> attrNames = new Dictionary<string, string>();
                attrNames.Add("Enabled", BoolToIntToString(itemEntry.Enabled));
                attrNames.Add("ItemId", itemEntry.ItemId.ToString());
                attrNames.Add("SellValue", itemEntry.SellValue.ToString());
                XmlCreateKey("Item", nodeAutoItemRegisterItems, attrNames);
            }
            XmlCreateKey("AutoItemBuy", XmlRoot, Settings.AutoItemBuy.List);
            foreach (var itemEntry in Settings.AutoItemBuy.Items)
            {
                Dictionary<string, string> attrNames = new Dictionary<string, string>();
                attrNames.Add("Enabled", BoolToIntToString(itemEntry.Enabled));
                attrNames.Add("ItemId", itemEntry.ItemId.ToString());
                attrNames.Add("EnchantLevel", itemEntry.EnchantLevel.ToString());
                attrNames.Add("SessionMax", itemEntry.SessionMax.ToString());
                attrNames.Add("MaxPrice", itemEntry.MaxPrice.ToString());
                attrNames.Add("IsStack", BoolToIntToString(itemEntry.IsStack));
                XmlCreateKey("Item", nodeAutoItemRegisterItems, attrNames);
            }
            XmlNode nodeOverlay = XmlCreateKey("Overlay", XmlRoot, Settings.Overlay.List);
            XmlNode nodeOverlayActors = XmlCreateKey("Actors", nodeOverlay);
            string commentActorTypes = "Types: ActorType_All, ActorType_Player, ActorType_Monster, ActorType_Npc, ActorType_Vehicle, ActorType_Gate, ActorType_Alterego, ActorType_Collect, ActorType_Household, ActorType_Installation, ActorType_Deadbody";
            XmlCreateKey(commentActorTypes, parentNode: nodeOverlayActors, comment: true);
            foreach (var itemEntry in Settings.Overlay.Actors)
            {
                Dictionary<string, string> attrNames = new Dictionary<string, string>();
                attrNames.Add("Enabled", BoolToIntToString(itemEntry.Enabled));
                attrNames.Add("Type", itemEntry.ActorType.ToString());
                attrNames.Add("IDs", String.Join(",", itemEntry.ActorIds));
                attrNames.Add("Color", String.Join(",", itemEntry.ColorCode));
                attrNames.Add("DrawCircle", BoolToIntToString(itemEntry.DrawCircle));
                attrNames.Add("DrawLine", BoolToIntToString(itemEntry.DrawLine));
                attrNames.Add("Thickness", itemEntry.Thickness.ToString());
                attrNames.Add("ShowName", BoolToIntToString(itemEntry.ShowName));
                attrNames.Add("ShowActorId", BoolToIntToString(itemEntry.ShowActorId));
                attrNames.Add("ShowDistance", BoolToIntToString(itemEntry.ShowDistance));
                attrNames.Add("ShowOnWorldMap", BoolToIntToString(itemEntry.ShowOnWorldMap));
                attrNames.Add("ShowLevel", BoolToIntToString(itemEntry.ShowLevel));
                attrNames.Add("ShowHp", BoolToIntToString(itemEntry.ShowHp));
                attrNames.Add("MinDistance", itemEntry.MinDistance.ToString());
                attrNames.Add("MaxDistance", itemEntry.MaxDistance.ToString());
                XmlCreateKey("Actor", nodeOverlayActors, attrNames);
            }
            XmlNode nodeOverlayWaypoints = XmlCreateKey("Waypoints", nodeOverlay);
            foreach (var itemEntry in Settings.Overlay.Waypoints)
            {
                Dictionary<string, string> attrNames = new Dictionary<string, string>();
                attrNames.Add("Enabled", BoolToIntToString(itemEntry.Enabled));
                attrNames.Add("Color", String.Join(",", itemEntry.ColorCode));
                attrNames.Add("Thickness", itemEntry.Thickness.ToString());
                attrNames.Add("Position", String.Join(",", itemEntry.Position));
                attrNames.Add("Name", itemEntry.Name);
                XmlCreateKey("Waypoint", nodeOverlayWaypoints, attrNames);
            }
            XmlNode nodeAutoProcessing = XmlCreateKey("AutoProcessing", XmlRoot, Settings.AutoProcessing.List);
            XmlNode nodeAutoProcessingItems = XmlCreateKey("AutoProcessing", nodeAutoProcessing);
            string commentProccessingTypes = "Types: Shaking, Grinding, Chopping, Drying, Thinning, Heating";
            XmlCreateKey(commentProccessingTypes, parentNode: nodeAutoProcessingItems, comment: true);
            foreach (var itemEntry in Settings.AutoProcessing.Items)
            {
                Dictionary<string, string> attrNames = new Dictionary<string, string>();
                attrNames.Add("Enabled", BoolToIntToString(itemEntry.Enabled));
                attrNames.Add("ItemId", itemEntry.ItemId.ToString());
                attrNames.Add("ProcessingType", itemEntry.ProcessingType.ToString());
                attrNames.Add("MinCount", itemEntry.MinCount.ToString());
                attrNames.Add("ResultItemIds", String.Join(",", itemEntry.ResultItemIds));
                attrNames.Add("Comment", itemEntry.Comment);
                XmlCreateKey("Item", nodeAutoProcessingItems, attrNames);
            }
            XmlDoc.Save(Files.fileSettings);
            return true;
        }

        public static void DumpSystemVariables()
        {
            XmlDocument xmlDoc = new XmlDocument();

            XmlNode Root = xmlDoc.CreateElement("SystemVariables");
            xmlDoc.AppendChild(Root);

            foreach (var sysvar in Collection.SystemVariables.Base.origList.List)
            {
                XmlNode child = xmlDoc.CreateElement(sysvar.Name);

                XmlAttribute attrib = xmlDoc.CreateAttribute("Enabled");
                attrib.Value = Convert.ToInt32(sysvar.Enabled).ToString();
                child.Attributes.Append(attrib);

                attrib = xmlDoc.CreateAttribute("ValueInt");
                attrib.Value = sysvar.ValueInt.ToString();
                child.Attributes.Append(attrib);

                attrib = xmlDoc.CreateAttribute("ValueFloat");
                attrib.Value = sysvar.ValueFloat.ToString();
                child.Attributes.Append(attrib);

                Root.AppendChild(child);
            }

            xmlDoc.Save(Files.fileSystemVariables);
        }

        public static void LoadSettings()
        {
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.Load(Files.fileSettings);

            XmlNode root = xmlDoc.FirstChild;

            XmlNode hotkeys = root["HotKeys"];

            if (hotkeys != null)
            {

                foreach (XmlAttribute atr in hotkeys.Attributes)
                {
                    Settings.HotKeys.List[atr.Name] = atr.Value;
                }
            }

            XmlNode autoFish = root["AutoFish"];

            if (autoFish != null)
            {

                foreach (XmlAttribute atr in autoFish.Attributes)
                {
                    Settings.AutoFish.List[atr.Name] = atr.Value;
                }
            }

            XmlNode autoRestore = root["AutoRestore"];

            if (autoRestore != null)
            {

                foreach (XmlAttribute atr in autoRestore.Attributes)
                {
                    Settings.AutoRestore.List[atr.Name] = atr.Value;
                }
            }

            XmlNode uiHack = root["UIHack"];

            if (uiHack != null)
            {

                foreach (XmlAttribute atr in uiHack.Attributes)
                {
                    Settings.UIHack.List[atr.Name] = atr.Value;
                }
            }

            XmlNode autoPot = root["AutoPotion"];

            if (autoPot != null)
            {

                foreach (XmlAttribute atr in autoPot.Attributes)
                {
                    Settings.AutoPotion.List[atr.Name] = atr.Value;
                }
            }

            XmlNode autoItemRegister = root["AutoItemRegister"];

            if (autoItemRegister != null)
            {

                foreach (XmlAttribute atr in autoItemRegister.Attributes)
                {
                    Settings.AutoItemRegister.List[atr.Name] = atr.Value;
                }


                XmlNode items = autoItemRegister["Items"];

                if (items != null)
                {

                    List<ItemRegisterObject> lItems = new List<ItemRegisterObject>();

                    foreach (XmlNode item in items.ChildNodes)
                    {
                        bool enabled = false;
                        if (item.Attributes.GetNamedItem("Enabled") != null)
                            enabled = Convert.ToBoolean(Convert.ToInt32(item.Attributes["Enabled"].Value));

                        int itemId = 0;
                        if (item.Attributes.GetNamedItem("ItemId") != null)
                            itemId = Convert.ToInt32(item.Attributes["ItemId"].Value);

                        int sellValue = 0;
                        if (item.Attributes.GetNamedItem("SellValue") != null)
                            sellValue = Convert.ToInt32(item.Attributes["SellValue"].Value);

                        lItems.Add(new ItemRegisterObject(enabled, itemId, sellValue));
                    }

                    Settings.AutoItemRegister.Items = lItems;
                }
            }

            XmlNode autoItemBuy = root["AutoItemBuy"];

            if (autoItemBuy != null)
            {

                foreach (XmlAttribute atr in autoItemBuy.Attributes)
                {
                    Settings.AutoItemBuy.List[atr.Name] = atr.Value;
                }


                XmlNode items = autoItemBuy["Items"];

                if (items != null)
                {

                    List<ItemBuyObject> lItems = new List<ItemBuyObject>();

                    foreach (XmlNode item in items.ChildNodes)
                    {
                        bool enabled = false;
                        if (item.Attributes.GetNamedItem("Enabled") != null)
                            enabled = Convert.ToBoolean(Convert.ToInt32(item.Attributes["Enabled"].Value));

                        int itemId = 0;
                        if (item.Attributes.GetNamedItem("ItemId") != null)
                            itemId = Convert.ToInt32(item.Attributes["ItemId"].Value);

                        int enchantLevel = 0;
                        if (item.Attributes.GetNamedItem("EnchantLevel") != null)
                            enchantLevel = Convert.ToInt32(item.Attributes["EnchantLevel"].Value);

                        int sessionMax = 0;
                        if (item.Attributes.GetNamedItem("SessionMax") != null)
                            sessionMax = Convert.ToInt32(item.Attributes["SessionMax"].Value);

                        long maxPrice = 0;
                        if (item.Attributes.GetNamedItem("MaxPrice") != null)
                            maxPrice = Convert.ToInt32(item.Attributes["MaxPrice"].Value);

                        bool isStack = false;
                        if (item.Attributes.GetNamedItem("IsStack") != null)
                            isStack = Convert.ToBoolean(Convert.ToInt32(item.Attributes["IsStack"].Value));

                        lItems.Add(new ItemBuyObject(enabled, itemId, enchantLevel, sessionMax, maxPrice, isStack));
                    }

                    Settings.AutoItemBuy.Items = lItems;
                }
            }

            XmlNode overlay = root["Overlay"];

            if (overlay != null)
            {

                foreach (XmlAttribute atr in overlay.Attributes)
                {
                    Settings.Overlay.List[atr.Name] = atr.Value;
                }
            }

            XmlNode actors = overlay["Actors"];

            if (actors != null)
            {

                List<ActorObject> lActors = new List<ActorObject>();

                foreach (XmlNode actor in actors.ChildNodes)
                {
                    if (actor.NodeType == XmlNodeType.Comment)
                        continue;

                    bool enabled = false;
                    if (actor.Attributes.GetNamedItem("Enabled") != null)
                        enabled = Convert.ToBoolean(Convert.ToInt32(actor.Attributes["Enabled"].Value));

                    ActorType actorType = ActorType.ActorType_All;
                    if (actor.Attributes.GetNamedItem("Type") != null)
                        actorType = (ActorType)Enum.Parse(typeof(ActorType), actor.Attributes["Type"].Value);



                    int[] actorIDs = new int[] { 0 };
                    if (actor.Attributes.GetNamedItem("IDs") != null)
                    {
                        List<int> lActorIDs = new List<int>();
                        var idSplit = actor.Attributes["IDs"].Value.Split(',');
                        foreach (var id in idSplit)
                        {
                            lActorIDs.Add(Convert.ToInt32(id));
                        }
                        actorIDs = lActorIDs.ToArray();
                    }

                    int[] color = new int[] { 255, 0, 0, 255 };
                    if (actor.Attributes.GetNamedItem("Color") != null)
                    {
                        List<int> lColor = new List<int>();
                        var colorSplit = actor.Attributes["Color"].Value.Split(',');
                        foreach (var cc in colorSplit)
                        {
                            lColor.Add(Convert.ToInt32(cc));
                        }
                        color = lColor.ToArray();
                    }

                    bool drawCircle = true;
                    if (actor.Attributes.GetNamedItem("DrawCircle") != null)
                        Convert.ToBoolean(Convert.ToInt32(actor.Attributes["DrawCircle"].Value));

                    bool drawLine = false;
                    if (actor.Attributes.GetNamedItem("DrawLine") != null)
                        drawLine = Convert.ToBoolean(Convert.ToInt32(actor.Attributes["DrawLine"].Value));

                    bool showName = false;
                    if (actor.Attributes.GetNamedItem("ShowName") != null)
                        showName = Convert.ToBoolean(Convert.ToInt32(actor.Attributes["ShowName"].Value));

                    bool showActorId = false;
                    if (actor.Attributes.GetNamedItem("ShowActorId") != null)
                        showActorId = Convert.ToBoolean(Convert.ToInt32(actor.Attributes["ShowActorId"].Value));

                    int thickness = 1;
                    if (actor.Attributes.GetNamedItem("Thickness") != null)
                        thickness = Convert.ToInt32(actor.Attributes["Thickness"].Value);

                    bool showDistance = false;
                    if (actor.Attributes.GetNamedItem("ShowDistance") != null)
                        showDistance = Convert.ToBoolean(Convert.ToInt32(actor.Attributes["ShowDistance"].Value));

                    bool showOnWorldMap = false;
                    if (actor.Attributes.GetNamedItem("ShowOnWorldMap") != null)
                        showOnWorldMap = Convert.ToBoolean(Convert.ToInt32(actor.Attributes["ShowOnWorldMap"].Value));

                    bool showLevel = false;
                    if (actor.Attributes.GetNamedItem("ShowLevel") != null)
                        showLevel = Convert.ToBoolean(Convert.ToInt32(actor.Attributes["ShowLevel"].Value));

                    bool showHp = false;
                    if (actor.Attributes.GetNamedItem("ShowHp") != null)
                        showHp = Convert.ToBoolean(Convert.ToInt32(actor.Attributes["ShowHp"].Value));

                    int minDistance = 0;
                    if (actor.Attributes.GetNamedItem("MinDistance") != null)
                        minDistance = Convert.ToInt32(actor.Attributes["MinDistance"].Value);

                    int maxDistance = 0;
                    if (actor.Attributes.GetNamedItem("MaxDistance") != null)
                        maxDistance = Convert.ToInt32(actor.Attributes["MaxDistance"].Value);

                    lActors.Add(new ActorObject(enabled, actorType, actorIDs, color, drawCircle, drawLine, thickness,
                        showName, showActorId, showDistance, showOnWorldMap, minDistance, maxDistance, showLevel, showHp));
                }

                Settings.Overlay.Actors = lActors;
            }

            XmlNode waypoints = overlay["Waypoints"];

            if (waypoints != null)
            {

                List<WaypointObject> lWaypoints = new List<WaypointObject>();

                foreach (XmlNode waypoint in waypoints.ChildNodes)
                {
                    if (waypoint.NodeType == XmlNodeType.Comment)
                        continue;

                    bool enabled = false;
                    if (waypoint.Attributes.GetNamedItem("Enabled") != null)
                        enabled = Convert.ToBoolean(Convert.ToInt32(waypoint.Attributes["Enabled"].Value));

                    int[] color = new int[] { 255, 0, 0, 255 };
                    if (waypoint.Attributes.GetNamedItem("Color") != null)
                    {
                        List<int> lColor = new List<int>();
                        var colorSplit = waypoint.Attributes["Color"].Value.Split(',');
                        foreach (var cc in colorSplit)
                        {
                            lColor.Add(Convert.ToInt32(cc));
                        }
                        color = lColor.ToArray();
                    }

                    int thickness = 1;
                    if (waypoint.Attributes.GetNamedItem("Thickness") != null)
                        thickness = Convert.ToInt32(waypoint.Attributes["Thickness"].Value);

                    float[] position = new float[] { 0f, 0f, 0f };
                    if (waypoint.Attributes.GetNamedItem("Position") != null)
                    {
                        List<float> lColor = new List<float>();
                        var colorSplit = waypoint.Attributes["Position"].Value.Split(',');
                        foreach (var cc in colorSplit)
                        {
                            lColor.Add(Convert.ToSingle(cc));
                        }
                        position = lColor.ToArray();
                    }

                    string name = "";
                    if (waypoint.Attributes.GetNamedItem("Name") != null)
                        name = waypoint.Attributes["Name"].Value;

                    lWaypoints.Add(new WaypointObject(enabled, color, thickness, position, name));
                }

                Settings.Overlay.Waypoints = lWaypoints;
            }

            XmlNode speedHack = root["SpeedHack"];

            if (speedHack != null)
            {

                foreach (XmlAttribute atr in speedHack.Attributes)
                {
                    Settings.SpeedHack.List[atr.Name] = atr.Value;
                }

                XmlNode horse = speedHack["Horse"];
                XmlNode ship = speedHack["Ship"];
                XmlNode player = speedHack["Player"];

                if (player != null)
                {

                    foreach (XmlAttribute atr in horse.Attributes)
                    {
                        Settings.SpeedHack.Horse.List[atr.Name] = atr.Value;
                    }

                    foreach (XmlAttribute atr in ship.Attributes)
                    {
                        Settings.SpeedHack.Ship.List[atr.Name] = atr.Value;
                    }

                    foreach (XmlAttribute atr in player.Attributes)
                    {
                        Settings.SpeedHack.Player.List[atr.Name] = atr.Value;
                    }
                }
            }

            XmlNode autoprocessing = root["AutoProcessing"];

            if (autoprocessing != null)
            {

                foreach (XmlAttribute atr in autoprocessing.Attributes)
                {
                    Settings.AutoProcessing.List[atr.Name] = atr.Value;
                }
            }

            XmlNode pItems = autoprocessing["Items"];

            if (pItems != null)
            {

                List<ProcessingObject> lpItems = new List<ProcessingObject>();

                foreach (XmlNode item in pItems.ChildNodes)
                {
                    if (item.NodeType == XmlNodeType.Comment)
                        continue;

                    bool enabled = false;
                    if (item.Attributes.GetNamedItem("Enabled") != null)
                        enabled = Convert.ToBoolean(Convert.ToInt32(item.Attributes["Enabled"].Value));

                    int itemId = 0;
                    if (item.Attributes.GetNamedItem("ItemId") != null)
                        itemId = Convert.ToInt32(item.Attributes["ItemId"].Value);

                    ProcessingType pType = ProcessingType.Chopping;
                    if (item.Attributes.GetNamedItem("ProcessingType") != null)
                        pType =
                            (ProcessingType)
                                Enum.Parse(typeof(ProcessingType), item.Attributes["ProcessingType"].Value);

                    int minCount = 10;
                    if (item.Attributes.GetNamedItem("MinCount") != null)
                        minCount = Convert.ToInt32(item.Attributes["MinCount"].Value);

                    int[] resultItemIds = new int[] { 0 };
                    if (item.Attributes.GetNamedItem("ResultItemIds") != null)
                    {
                        List<int> lb = new List<int>();
                        var idSplit = item.Attributes["ResultItemIds"].Value.Split(',');
                        foreach (var id in idSplit)
                        {
                            lb.Add(Convert.ToInt32(id));
                        }
                        resultItemIds = lb.ToArray();
                    }

                    string comment = "";
                    if (item.Attributes.GetNamedItem("Comment") != null)
                        comment = item.Attributes["Comment"].Value;

                    lpItems.Add(new ProcessingObject(enabled, itemId, pType, minCount, resultItemIds, comment));
                }

                Settings.AutoProcessing.Items = lpItems;
            }

            SaveSettings();
            }

        public static void LoadSystemVariables()
        {
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.Load(Files.fileSystemVariables);

            XmlNode root = xmlDoc.FirstChild;

            int index = 0;

            foreach (XmlNode child in root.ChildNodes)
            {
                string name = child.Name;
                bool enabled = Convert.ToBoolean(Convert.ToInt32(child.Attributes["Enabled"].Value));
                int valueInt = Convert.ToInt32(child.Attributes["ValueInt"].Value);
                float valueFloat = Convert.ToSingle(child.Attributes["ValueFloat"].Value);

                SystemVariable origSysVar = Collection.SystemVariables.Base.origList.List.FirstOrDefault(x => x.Name == name);

                if (origSysVar != null)
                {
                    if (origSysVar.Enabled != enabled)
                        origSysVar.SetEnabled((byte)Convert.ToInt32(enabled));

                    if (origSysVar.ValueInt != valueInt)
                        origSysVar.SetValue(valueInt);

                    if (!origSysVar.ValueFloat.Equals(valueFloat))
                        origSysVar.SetValue(valueFloat);
                }

                index++;
            }
        }
    }
}
