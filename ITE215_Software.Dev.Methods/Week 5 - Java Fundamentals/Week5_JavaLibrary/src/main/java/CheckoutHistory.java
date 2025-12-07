import org.json.JSONException;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.*;

public class CheckoutHistory
{
    private final List<CheckoutRecord> HistoryLog; // simulates the database of history log

    public CheckoutHistory()
    {
        HistoryLog = new ArrayList<>();
    }

    public void AddToLog(CheckoutRecord record)
    {
        HistoryLog.add(record);
        try
        {
            SaveToDisk();
        }catch (Exception e) { /**/ }
    }

    public List<CheckoutRecord> GetHistory()
    {
        return this.HistoryLog.stream().toList();
    }


    private static final String DiskPath = ".\\History.json";

    public void SaveToDisk()
    {
        try {
            final var json = new org.json.JSONObject();
            var array = new org.json.JSONArray();

            // Write members to disk
            for (var item : HistoryLog) {
                array.put(item.toJSON());
            }
            json.put("History", array);

            Files.writeString(Path.of(DiskPath), json.toString(4));
        } catch (Exception e) {
            /**/
        }
    }

    public static CheckoutHistory LoadFromDisk()
    {
        var log = new CheckoutHistory();

        try {
            String content = Files.readString(Path.of(DiskPath));
            var obj = new org.json.JSONObject(content);

            // ISBNS
            try {
                var details = obj.getJSONArray("History");
                for (int i = 0; i < details.length(); i++) {
                    var jItem = details.getJSONObject(i);
                    var item = CheckoutRecord.fromJSON(jItem);
                    log.HistoryLog.add(item);
                }
            } catch (JSONException j) {
                // ignore
            }
        } catch (IOException e) {
            return log;
        }
        return log;
    }
}
