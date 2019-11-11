using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class MainMenu : MonoBehaviour
{

    private bool loadScene = false;
    [SerializeField]
    private Text loadingText;
    [SerializeField]
    private GameObject controls;
    [SerializeField]
    private Text title;
    [SerializeField]
    private GameObject start;
    [SerializeField]
    private GameObject controlsButton;
    [SerializeField]
    private GameObject backButton;



    // Update is called once per frame
    void Update()
    {
        if(loadScene == true)
        {
            loadingText.color = new Color(loadingText.color.r, loadingText.color.g, loadingText.color.b, Mathf.PingPong(Time.time, 1));
        }
    }

    public void StartLoadingScene()
    {
        if(loadScene == false)
        {
            loadScene = true;

            loadingText.text = "Loading...";

            StartCoroutine(LoadNewScene());

            title.enabled = false;
            start.SetActive(false);
            controlsButton.SetActive(false);
        }
       
    }
    IEnumerator LoadNewScene()
    {

        AsyncOperation async = SceneManager.LoadSceneAsync("Shooter");

        while (!async.isDone)
        {
            yield return null;
        }
    }
    public void controlsShow()
    {
        title.enabled = false;
        start.SetActive(false);
        controlsButton.SetActive(false);
        backButton.SetActive(true);
        controls.SetActive(true);
    }
    public void returnScreen()
    {
        title.enabled = true;
        start.SetActive(true);
        controlsButton.SetActive(true);
        backButton.SetActive(false);
        controls.SetActive(false);
    }

}
