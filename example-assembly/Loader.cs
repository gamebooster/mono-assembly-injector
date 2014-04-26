using UnityEngine;

namespace HearthstoneSpoofer
{
    public class Loader {
        public static void Load() {
          SceneMgr.Get().gameObject.AddComponent<VariousUtils>();
        }

        public static void Unload() {
          var utils = SceneMgr.Get().gameObject.GetComponent<VariousUtils>();
          if (utils != null) {
            UnityEngine.Object.DestroyImmediate(utils);
          }
        }
    }
}
