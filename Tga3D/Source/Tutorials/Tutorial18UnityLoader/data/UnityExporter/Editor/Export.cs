using UnityEngine;
using UnityEditor;
using System.Collections;
using System.Collections.Generic;

public class MenuItems
{
    [MenuItem("Tools/Export all #_x")]
    private static void NewMenuOption()
    {
        using (System.IO.StreamWriter file = new System.IO.StreamWriter("exportedLevelASCII.txt"))
        {
            GameObject[] allObjects = UnityEngine.Object.FindObjectsOfType<GameObject>();


            ListContainer objects = new ListContainer();
            objects.dataList = new List<ObjectData>();
            //List<ObjectData> savingObjects = new List<ObjectData>();
            foreach (GameObject go in allObjects)
            {
                ObjectData savingObject = new ObjectData();
                savingObject.myRotX = go.transform.rotation.eulerAngles.x;
                savingObject.myRotY = go.transform.rotation.eulerAngles.y;
                savingObject.myRotZ = go.transform.rotation.eulerAngles.z;
                savingObject.myPosX = go.transform.position.x;
                savingObject.myPosY = go.transform.position.y;
                savingObject.myPosZ = go.transform.position.z;

                savingObject.myScaleX = go.transform.localScale.x;
                savingObject.myScaleY = go.transform.localScale.y;
                savingObject.myScaleZ = go.transform.localScale.z;

                savingObject.myName = go.name;
                savingObject.myPath = AssetDatabase.GetAssetPath(PrefabUtility.GetCorrespondingObjectFromSource(go));
                objects.dataList.Add(savingObject);
            }
            string jsonString = JsonUtility.ToJson(objects, true);
            file.WriteLine(jsonString);
        }
    }

    [System.Serializable]
    public class ObjectData
    {
        public string myName;
        public float myRotX;
        public float myRotY;
        public float myRotZ;
        public float myPosX;
        public float myPosY;
        public float myPosZ;
        public float myScaleX;
        public float myScaleY;
        public float myScaleZ;
        public string myPath;
    }

    [System.Serializable]
    public struct ListContainer
    {
        public List<ObjectData> dataList;
        public ListContainer(List<ObjectData> aDataList)
        {
            dataList = aDataList;
        }
    }

}
