using System.Collections.Generic;
using System.Reflection;
using System.Timers;

using UnityEngine;
using Random = System.Random;

namespace HearthstoneSpoofer {
  public class VariousUtils : MonoBehaviour {
    private readonly Timer timer_ = new Timer();

    private readonly Timer second_timer_ = new Timer();
    private readonly Random random_ = new Random();

    private void Start() {
      timer_.Interval = 500;
      timer_.AutoReset = true;
      timer_.Elapsed += timer_Elapsed;
      timer_.Start();
    }


    private void timer_Elapsed(object sender, ElapsedEventArgs e) {
      NetCache.Get().GetNetObject<NetCache.NetCacheFeatures>().Games.ShowUserUI = 0;
    }

    private void OnGUI() {
      GUI.Label(new Rect(10, 10, 200, 200), "active");

      if (GameState.Get() != null && GameState.Get().GetRemotePlayer() != null) {
        var account = GameState.Get().GetRemotePlayer().GetBnetPlayer().GetBestGameAccount();
        string tag = account.GetBattleTag().GetString();
        var field = typeof(Gameplay).GetField("m_nameBanners", BindingFlags.NonPublic | BindingFlags.GetField | BindingFlags.Instance);
        var name_banners = (List<NameBanner>)field.GetValue(Gameplay.Get());
        foreach (var banner in name_banners) {
          if (!banner.IsLocal()) {
            banner.SetName(tag);
          }
        }
      }
    }
  }
}
      
